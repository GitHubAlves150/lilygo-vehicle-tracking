#include "../include/modem.h"
#include "../include/hardware.h"
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"

void modem_task(void *pvParameters) {
    uart_config_t modem_config = {};
    modem_config.baud_rate = 115200;
    modem_config.data_bits = UART_DATA_8_BITS;
    modem_config.parity    = UART_PARITY_DISABLE;
    modem_config.stop_bits = UART_STOP_BITS_1;
    modem_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    modem_config.source_clk = UART_SCLK_DEFAULT;

    ESP_ERROR_CHECK(uart_driver_install(MODEM_UART_NUM, UART_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(MODEM_UART_NUM, &modem_config));
    ESP_ERROR_CHECK(uart_set_pin(MODEM_UART_NUM, BOARD_MODEM_TX_PIN, BOARD_MODEM_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    uint8_t *data = (uint8_t *) malloc(UART_BUF_SIZE);

    while (1) {
        int len = uart_read_bytes(MODEM_UART_NUM, data, UART_BUF_SIZE - 1, pdMS_TO_TICKS(100));
        if (len > 0) {
            data[len] = '\0';
            printf("%s", (char*)data);
            fflush(stdout);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    free(data);
    vTaskDelete(NULL);
}