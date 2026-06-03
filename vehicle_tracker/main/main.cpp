#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "include/hardware.h"
#include "include/gps_parser.h"
#include "include/modem.h"

extern "C" void app_main(void) {
    printf("\n");
    printf("========================================\n");
    printf("   RASTREADOR VEICULAR - LILYGO T-A7670\n");
    printf("   Versao 2.0 - Modular Organizado\n");
    printf("========================================\n\n");

    // 1. Inicializa pinos e sequência de Boot da Placa LilyGO
    init_board_power();
    
    // Aguarda a estabilização elétrica do modem periférico antes de abrir os canais
    vTaskDelay(pdMS_TO_TICKS(2000));

    // 2. Dispara as Tarefas paralelas (Multithreading nativo do ESP-IDF)
    xTaskCreate(gps_task, "gps_task", 4096, NULL, 5, NULL);
    xTaskCreate(modem_task, "modem_task", 4096, NULL, 4, NULL);
}