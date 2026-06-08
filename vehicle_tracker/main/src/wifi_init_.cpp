#include "../include/wifi_init_.h"   
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"  
#include "esp_netif.h"

#define WIFI_SSID       "KINGKONG_ES"
#define WIFI_PASS       "12345678"

static const char *TAG = "WIFI_INIT";
static bool wifi_ready = false;

void wifi_event_handler(void* arg, esp_event_base_t event_base,
                        int32_t event_id, void* event_data)
{
    ESP_LOGI(TAG, "📡 Evento recebido: base=%s, id=%ld", event_base, event_id);
    
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "Conectando ao WiFi...");
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGW(TAG, "WiFi desconectado. Tentando reconectar...");
        esp_wifi_connect();
        wifi_ready = false;
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "🎉 IP_EVENT_STA_GOT_IP recebido! IP: " IPSTR, IP2STR(&event->ip_info.ip));
        wifi_ready = true;
    }
    else {
        ESP_LOGI(TAG, "Outro evento ignorado: base=%s, id=%ld", event_base, event_id);
    }
}

bool is_wifi_ready(void) {
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif) {
        esp_netif_ip_info_t ip_info;
        if (esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
            if (ip_info.ip.addr != 0) {
                return true;
            }
        }
    }
    return false;
}

void wifi_init(void) {
    ESP_LOGI(TAG, "Inicializando WiFi...");
    
    // 1. INICIALIZAÇÃO DA MEMÓRIA FLASH NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    // 🔥 2. ADICIONE ESTA LINHA OBRIGATÓRIA (Inicializa a pilha de rede)
    ESP_ERROR_CHECK(esp_netif_init());
    
    // 3. CRIAÇÃO DO LOOP DE EVENTOS DO SISTEMA
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    // 🔥 4. ADICIONE ESTA LINHA OBRIGATÓRIA (Cria a interface padrão do Wi-Fi Station)
    esp_netif_create_default_wifi_sta();
    
    // 5. CONFIGURAÇÃO E START DO WI-FI (O restante continua igual)
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));
    
    wifi_config_t wifi_config = {};
    strcpy((char*)wifi_config.sta.ssid, WIFI_SSID);
    strcpy((char*)wifi_config.sta.password, WIFI_PASS);
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    
    ESP_LOGI(TAG, "WiFi inicializado.");
}