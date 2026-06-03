#include "../include/gps_parser.h"
#include "../include/hardware.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"

static const char *TAG = "GPS_PARSER";
static gps_data_t current_gps = {0.0, 0.0, "", "", false};

void parse_nmea_gprmc(const char *line) {
    if (strncmp(line, "$GPRMC", 6) == 0 || strncmp(line, "$GNRMC", 6) == 0) {
        char status;
        char lat_dir, lon_dir;
        double raw_lat = 0.0, raw_lon = 0.0;
        char time_str[16] = {0};
        char date_str[16] = {0};

        int parsed = sscanf(line, "%*[^,],%[^,],%c,%lf,%c,%lf,%c,%*[^,],%*[^,],%[^,]", 
                            time_str, &status, &raw_lat, &lat_dir, &raw_lon, &lon_dir, date_str);

        if (parsed >= 6 && status == 'A') {
            int lat_degrees = (int)(raw_lat / 100);
            double lat_minutes = raw_lat - (lat_degrees * 100);
            current_gps.latitude = lat_degrees + (lat_minutes / 60.0);
            if (lat_dir == 'S') current_gps.latitude = -current_gps.latitude;

            int lon_degrees = (int)(raw_lon / 100);
            double lon_minutes = raw_lon - (lon_degrees * 100);
            current_gps.longitude = lon_degrees + (lon_minutes / 60.0);
            if (lon_dir == 'W') current_gps.longitude = -current_gps.longitude;

            current_gps.valid = true;
            snprintf(current_gps.fix_time, sizeof(current_gps.fix_time), "%.6s", time_str);
            snprintf(current_gps.fix_date, sizeof(current_gps.fix_date), "%.6s", date_str);

            ESP_LOGI(TAG, "Location: %.6f, %.6f | Hora UTC: %s | Data: %s", 
                     current_gps.latitude, current_gps.longitude, current_gps.fix_time, current_gps.fix_date);
        } else {
            current_gps.valid = false;
            ESP_LOGW(TAG, "Location: INVALID (Aguardando sinal dos satélites...)");
        }
    }
}

void gps_task(void *pvParameters) {
    uart_config_t gps_config = {};
    gps_config.baud_rate = 9600;
    gps_config.data_bits = UART_DATA_8_BITS;
    gps_config.parity    = UART_PARITY_DISABLE;
    gps_config.stop_bits = UART_STOP_BITS_1;
    gps_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    gps_config.source_clk = UART_SCLK_DEFAULT;

    ESP_ERROR_CHECK(uart_driver_install(GPS_UART_NUM, UART_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(GPS_UART_NUM, &gps_config));
    ESP_ERROR_CHECK(uart_set_pin(GPS_UART_NUM, BOARD_GPS_TX_PIN, BOARD_GPS_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    uint8_t *data = (uint8_t *) malloc(UART_BUF_SIZE);
    char line_buffer[LINE_BUF_SIZE];
    int line_index = 0;

    while (1) {
        int len = uart_read_bytes(GPS_UART_NUM, data, UART_BUF_SIZE - 1, pdMS_TO_TICKS(100));
        if (len > 0) {
            for (int i = 0; i < len; i++) {
                char c = (char)data[i];
                if (c == '\n' || c == '\r') {
                    if (line_index > 0) {
                        line_buffer[line_index] = '\0';
                        parse_nmea_gprmc(line_buffer);
                        line_index = 0;
                    }
                } else if (line_index < (LINE_BUF_SIZE - 1)) {
                    line_buffer[line_index++] = c;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    free(data);
    vTaskDelete(NULL);
}