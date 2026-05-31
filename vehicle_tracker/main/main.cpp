#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"



static const char *TAG = "VEHICLE_TRACKER";

// Configuração da UART para o GPS (A7670E)
#define GPS_UART_NUM UART_NUM_1
#define GPS_TXD_PIN 17
#define GPS_RXD_PIN 16
#define GPS_BAUD_RATE 9600

extern "C" void app_main(void)
{
    printf("\n");
    printf("========================================\n");
    printf("   LILYGO VEHICLE TRACKER - ESP32\n");
    printf("   Versao: 1.0.0\n");
    printf("========================================\n\n");
    
    // Configura UART para comunicação com GPS
    uart_config_t uart_config = {
        .baud_rate = GPS_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    
    uart_param_config(GPS_UART_NUM, &uart_config);
    uart_set_pin(GPS_UART_NUM, GPS_TXD_PIN, GPS_RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(GPS_UART_NUM, 1024, 0, 0, NULL, 0);
    
    ESP_LOGI(TAG, "GPS UART configurado na GPIO16(RX) e GPIO17(TX)");
    ESP_LOGI(TAG, "Aguardando dados do modulo A7670E...");
    
    while (1) {
        // Loop principal do rastreador
        uint8_t data[256];
        int len = uart_read_bytes(GPS_UART_NUM, data, sizeof(data), 100 / portTICK_PERIOD_MS);
        
        if (len > 0) {
            // Processa dados do GPS (NMEA)
            for (int i = 0; i < len; i++) {
                putchar(data[i]);  // Imprime no terminal para debug
            }
        }
        
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}