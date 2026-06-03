#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "include/hardware.h"
#include "include/gps_parser.h"
#include "include/modem.h"
#include "include/wifi_init_.h"
#include "esp_log.h"



static const char *TAG = "MAIN";



extern "C" void app_main(void) {
    printf("\n");
    printf("========================================\n");
    printf("   RASTREADOR VEICULAR - LILYGO T-A7670\n");
    printf("   Versao 3.0 - Com WiFi\n");
    printf("========================================\n\n");

    // 1. Inicializa WiFi (antes do GPS)
    wifi_init();
    
    
    // 2. Inicializa hardware (liga alimentação)
    init_board_power();
    vTaskDelay(pdMS_TO_TICKS(2000));

    // 3. Inicia as tasks
    xTaskCreate(gps_task, "gps_task", 4096, NULL, 5, NULL);
    xTaskCreate(modem_task, "modem_task", 4096, NULL, 4, NULL);

    ESP_LOGI(TAG, "Sistema em execucao - Aguardando GPS...");
}