#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

static const char *TAG = "VEHICLE_TRACKER";

// Definições de Hardware extraídas do seu código Arduino
#define BOARD_MODEM_TX_PIN                  26
#define BOARD_MODEM_RX_PIN                  27
#define BOARD_MODEM_PWR_PIN                 4
#define BOARD_POWER_ON_PIN                  12
#define BOARD_RST_PIN                       5

#define BOARD_GPS_TX_PIN                    21
#define BOARD_GPS_RX_PIN                    22

// Portas UART do ESP32
#define MODEM_UART_NUM                      UART_NUM_1
#define GPS_UART_NUM                        UART_NUM_2
#define UART_BUF_SIZE                       1024
#define LINE_BUF_SIZE                       128

// Estrutura para armazenar os dados tratados do GPS (Substituindo TinyGPS++)
typedef struct {
    double latitude;
    double longitude;
    char fix_time[9];
    char fix_date[7];
    bool valid;
} gps_data_t;

gps_data_t current_gps = {0.0, 0.0, "", "", false};

// Função para processar sentenças GPRMC/GNRMC e extrair as coordenadas
void parse_nmea_gprmc(const char *line) {
    // Exemplo: $GPRMC,123456.00,A,4512.3456,N,0122.3456,E,...
    if (strncmp(line, "$GPRMC", 6) == 0 || strncmp(line, "$GNRMC", 6) == 0) {
        char status;
        char lat_dir, lon_dir;
        double raw_lat = 0.0, raw_lon = 0.0;
        char time_str[16] = {0};
        char date_str[16] = {0};

        // Faz o parse seguro baseado na estrutura da string NMEA RMC
        int parsed = sscanf(line, "%*[^,],%[^,],%c,%lf,%c,%lf,%c,%*[^,],%*[^,],%[^,]", 
                            time_str, &status, &raw_lat, &lat_dir, &raw_lon, &lon_dir, date_str);

        if (parsed >= 6 && status == 'A') { // 'A' significa dado Ativo/Válido
            // Converte o formato do GPS (DDMM.MMMM) para Graus Decimais (DD.DDDDDD)
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

            // Exibe os dados formatados (Equivalente ao displayInfo do Arduino)
            ESP_LOGI(TAG, "Location: %.6f, %.6f | Hora UTC: %s | Data: %s", 
                     current_gps.latitude, current_gps.longitude, current_gps.fix_time, current_gps.fix_date);
        } else {
            current_gps.valid = false;
            ESP_LOGW(TAG, "Location: INVALID (Aguardando sinal dos satélites...)");
        }
    }
}

// Inicializa os pinos de alimentação da placa LilyGO
void init_board_power(void) {
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = ((1ULL << BOARD_POWER_ON_PIN) | (1ULL << BOARD_RST_PIN) | (1ULL << BOARD_MODEM_PWR_PIN));
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    // Liga a alimentação geral da placa
    gpio_set_level((gpio_num_t)BOARD_POWER_ON_PIN, 1);
    gpio_set_level((gpio_num_t)BOARD_RST_PIN, 0);
    
    // Sequência de Pulso no pino PWR para iniciar o Modem/GPS integrado
    gpio_set_level((gpio_num_t)BOARD_MODEM_PWR_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level((gpio_num_t)BOARD_MODEM_PWR_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(1000));
    gpio_set_level((gpio_num_t)BOARD_MODEM_PWR_PIN, 0);
    
    ESP_LOGI(TAG, "Hardware energizado e pinos configurados.");
}

// Task para monitorar os dados vindos do GPS
static void gps_task(void *pvParameters) {
    uart_config_t gps_config = {};
    gps_config.baud_rate = 9600;
    gps_config.data_bits = UART_DATA_8_BITS;
    gps_config.parity    = UART_PARITY_DISABLE;
    gps_config.stop_bits = UART_STOP_BITS_1;
    gps_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    gps_config.source_clk = UART_SCLK_DEFAULT;

    ESP_ERROR_CHECK(uart_driver_install(GPS_UART_NUM, UART_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(GPS_UART_NUM, &gps_config));
    // TX = 21, RX = 22
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

// Task para monitorar e enviar comandos AT ao modem
static void modem_task(void *pvParameters) {
    uart_config_t modem_config = {};
    modem_config.baud_rate = 115200;
    modem_config.data_bits = UART_DATA_8_BITS;
    modem_config.parity    = UART_PARITY_DISABLE;
    modem_config.stop_bits = UART_STOP_BITS_1;
    modem_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    modem_config.source_clk = UART_SCLK_DEFAULT;

    ESP_ERROR_CHECK(uart_driver_install(MODEM_UART_NUM, UART_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(MODEM_UART_NUM, &modem_config));
    // TX = 26, RX = 27
    ESP_ERROR_CHECK(uart_set_pin(MODEM_UART_NUM, BOARD_MODEM_TX_PIN, BOARD_MODEM_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    uint8_t *data = (uint8_t *) malloc(UART_BUF_SIZE);

    while (1) {
        int len = uart_read_bytes(MODEM_UART_NUM, data, UART_BUF_SIZE - 1, pdMS_TO_TICKS(100));
        if (len > 0) {
            data[len] = '\0';
            // Imprime no console principal tudo que o modem responder de comandos AT
            printf("%s", (char*)data);
            fflush(stdout);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    free(data);
    vTaskDelete(NULL);
}

extern "C" void app_main(void) {
    // 1. Inicializa pinos e sequência de Boot da Placa LilyGO
    init_board_power();
    
    // Aguarda a estabilização elétrica do modem periférico antes de abrir os canais
    vTaskDelay(pdMS_TO_TICKS(2000));

    // 2. Dispara as Tarefas paralelas (Multithreading nativo do ESP-IDF)
    xTaskCreate(gps_task, "gps_task", 4096, NULL, 5, NULL);
    xTaskCreate(modem_task, "modem_task", 4096, NULL, 4, NULL);
}
