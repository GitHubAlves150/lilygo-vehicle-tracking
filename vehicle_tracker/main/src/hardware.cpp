#include "../include/hardware.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "HARDWARE";

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