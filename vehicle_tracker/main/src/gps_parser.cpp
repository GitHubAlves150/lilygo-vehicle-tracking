#include "../include/gps_parser.h"
#include "../include/hardware.h"
#include "../include/wifi_init_.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "esp_wifi.h" // ← ADICIONAR para wifi_is_connected
#include "esp_crt_bundle.h" // 🔥 ADICIONE ESTA LINHA NO TOPO

static const char *TAG = "GPS_PARSER";

// Certificado Raiz Global da AWS (Amazon Root CA 1)
// Substitua o seu bloco antigo do amazon_root_ca por este:
const char *amazon_root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDQTCCAimgAwIBAgITBmyfz5mMgVoEEWS7IX924Anf/jANBgkqhkiG9w0BAQsF\n"
    "ADCBmDELMAkGA1UEBhMCVVMxEDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTUGhv\n"
    "ZW5peDEJMCcGA1UEChMgQW1hem9uLmNvbSBJbmMuMRswGQYDVQQLExJBbWF6b24g\n"
    "Q2VydGlmaWNhdGlvbiBBdXRob3JpdHkxJTAjBgNVBAMTHEFtYXpvbiBSb290IENB\n"
    "IDEwHhcNMTUwNTI2MDAwMDAwWhcNMzgwNTI2MDAwMDAwWjCBmDELMAkGA1UEBhMC\n"
    "VVMxEDAOBgNVBAgTB0FyaXpvbmExEzARBgNVBAcTUGhvZW5peDEJMCcGA1UEChMg\n"
    "QW1hem9uLmNvbSBJbmMuMRswGQYDVQQLExJBbWF6b24gQ2VydGlmaWNhdGlvbiBB\n"
    "dXRob3JpdHkxJTAjBgNVBAMTHEFtYXpvbiBSb290IENBIDEwggEiMA0GCSqGSIb3\n"
    "DQEBAQUAA4IBDwAwggEKAoIBAQDJ65MZ9EtTjsbWvTbc7Ew9tLQUtTf8s5O8FQed\n"
    "c6XfJuMGoLfVEiPWHwUs4BBKWj0Q4FOCosUKLC5mfxbO1GcV3fi9RVafVUdCwGSZ\n"
    "fBK9649bkaf26ba9O8aUxguCrw0bdChWWtdgHXm89B218FPbhk6pm5OUANMm6h3y\n"
    "jAI9aAVRSEHIF9qi1dnvMGm7m8W3Ka86w14Ym9stnSYjYWscfdGdSgNNREgB+6ZO\n"
    "A3gWIK8ghYKWuMWhA3U64W9E6GKB7RSuA1mupS06m74hZzRFrUkz89m88uFv5y0E\n"
    "GUVDo99m4p6vE0Nf/NfL8U689bYbg5KUGpHiS8Z3Y6Y7MDF4AwEAATNoMRYwFAYD\n"
    "VR0RBA0wC4IJbG9jYWxob3N0MA4GA1UdDwEB/wQEAwIBhjAPBgNVHRMBAf8EBTAD\n"
    "AQH/MB0GA1UdDgQWBBUQURRGsCHZd9FILLFdaJe5gGDw9DAfBgNVHSMEGDAWgBUQ\n"
    "URRGsCHZd9FILLFdaJe5gGDw9DAzBgNVHR8ELDAqMCigJqAkhiJodHRwOi8vY3Js\n"
    "LmFtYXpvbnRydXN0LmNvbS9yb290Y2ExLmNybDARBgNVHSAECjAIMAYGBFUdIAAw\n"
    "DQYJKoZIhvcNAQELBQADggEBAIvR3898G8G98bA/pYxK5yG4WJ8dD0TjF6A2V9M/\n"
    "N86oY7Z7yZ0mkyj1EId2p0M8O9564fK1C57N3F+E9mD0o5uYm4GqCymxIDJ9f3eK\n"
    "wzAnr6QyA9XNhT7k6XnL1i6F5i87jC2kYyQG+v1hWJ5Nfe5eG8p/FzKOfXQZWhqR\n"
    "E34/uN6/u5V+wYn773n9F96y1A/rF5q1V6mGpHmS8Z3Y6Y7MDF4A==\n"
    "-----END CERTIFICATE-----\n";

// Estrutura de dados do GPS
gps_data_t current_gps = {0.0, 0.0, 0.0, "", "", false}; // ← Adicionado campo speed

// URL da API
static const char *API_URL = "https://g1rwfyb8el.execute-api.us-east-2.amazonaws.com/default/VehicleTelemetryReceiver";

// Função para enviar dados para a API
static void enviarDadosGPS(double lat, double lon, double speed, const char *vehicle_id)
{
    // 1. Controle de tempo para não inundar a rede (5 segundos)
    static uint32_t ultimo_envio = 0;
    uint32_t tempo_atual = xTaskGetTickCount() * portTICK_PERIOD_MS;

    if (tempo_atual - ultimo_envio < 5000)
    {
        return;
    }
    ultimo_envio = tempo_atual;

    // 2. Verifica se a rede está pronta
    if (!is_wifi_ready())
    {
        ESP_LOGW(TAG, "WiFi ainda não está pronto. Dados não enviados.");
        return;
    }

    // 3. Verifica coordenadas válidas
    if (lat == 0.0 && lon == 0.0)
    {
        ESP_LOGW(TAG, "Coordenadas inválidas, não enviando");
        return;
    }

    // 4. Constrói o JSON manualmente
    char json_buffer[256];
    snprintf(json_buffer, sizeof(json_buffer),
             "{\"vehicle_id\":\"%s\",\"latitude\":%.6f,\"longitude\":%.6f,\"speed\":%.1f}",
             vehicle_id, lat, lon, speed);

    ESP_LOGI(TAG, "📤 Enviando para AWS: %s", json_buffer);

    // 5. Configuração do Cliente HTTP integrada ao Bundle TLS do ESP-IDF v5.3
    esp_http_client_config_t config = {}; 
    config.url = API_URL;
    config.method = HTTP_METHOD_POST;
    config.timeout_ms = 5000;
    config.crt_bundle_attach = esp_crt_bundle_attach; // Usa os certificados globais embutidos nativos

    // 6. APENAS UMA declaração do client aqui:
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client == NULL) {
        ESP_LOGE(TAG, "Falha ao inicializar o cliente HTTP");
        return;
    }

    // 7. Define os cabeçalhos e o payload de transmissão
    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, json_buffer, strlen(json_buffer));

    // 8. Executa a requisição síncrona HTTPS
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK)
    {
        int status_code = esp_http_client_get_status_code(client);
        if (status_code == 200 || status_code == 201)
        {
            ESP_LOGI(TAG, "✅ Dados enviados com sucesso para a AWS!");
        }
        else
        {
            ESP_LOGW(TAG, "⚠️ AWS respondeu com código: %d", status_code);
        }
    }
    else
    {
        ESP_LOGE(TAG, "❌ Falha no envio: %s", esp_err_to_name(err));
    }

    // 9. Liberação do descritor de memória
    esp_http_client_cleanup(client);
}

void parse_nmea_gprmc(const char *line)
{
    if (strncmp(line, "$GPRMC", 6) == 0 || strncmp(line, "$GNRMC", 6) == 0)
    {
        char status;
        char lat_dir, lon_dir;
        double raw_lat = 0.0, raw_lon = 0.0;
        double raw_speed = 0.0; // ← ADICIONAR campo para velocidade
        char time_str[16] = {0};
        char date_str[16] = {0};

        // ← MODIFICAR o sscanf para incluir a velocidade
        int parsed = sscanf(line, "%*[^,],%[^,],%c,%lf,%c,%lf,%c,%lf,%*[^,],%[^,]",
                            time_str, &status, &raw_lat, &lat_dir, &raw_lon, &lon_dir, &raw_speed, date_str);

        if (parsed >= 6 && status == 'A')
        {
            int lat_degrees = (int)(raw_lat / 100);
            double lat_minutes = raw_lat - (lat_degrees * 100);
            current_gps.latitude = lat_degrees + (lat_minutes / 60.0);
            if (lat_dir == 'S')
                current_gps.latitude = -current_gps.latitude;

            int lon_degrees = (int)(raw_lon / 100);
            double lon_minutes = raw_lon - (lon_degrees * 100);
            current_gps.longitude = lon_degrees + (lon_minutes / 60.0);
            if (lon_dir == 'W')
                current_gps.longitude = -current_gps.longitude;

            // ← ADICIONAR velocidade (converter nós para km/h)
            current_gps.speed = raw_speed * 1.852;

            current_gps.valid = true;
            snprintf(current_gps.fix_time, sizeof(current_gps.fix_time), "%.6s", time_str);
            snprintf(current_gps.fix_date, sizeof(current_gps.fix_date), "%.6s", date_str);

            ESP_LOGI(TAG, "📍 Posição: %.6f, %.6f | Vel: %.1f km/h | %s | %s",
                     current_gps.latitude, current_gps.longitude,
                     current_gps.speed,
                     current_gps.fix_time, current_gps.fix_date);

            // ← ADICIONE ESTA LINHA PARA DEBUG
            ESP_LOGI(TAG, "🔍 Chamando enviarDadosGPS...");

            // Envia para a API
            enviarDadosGPS(current_gps.latitude, current_gps.longitude,
                           current_gps.speed, "lilygo-vehicle-tracking");
        }
        else
        {
            current_gps.valid = false;
            ESP_LOGW(TAG, "🛰️ Aguardando sinal GPS...");
        }
    }
}

void gps_task(void *pvParameters)
{

    uart_config_t gps_config = {};
    gps_config.baud_rate = 9600;
    gps_config.data_bits = UART_DATA_8_BITS;
    gps_config.parity = UART_PARITY_DISABLE;
    gps_config.stop_bits = UART_STOP_BITS_1;
    gps_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    gps_config.source_clk = UART_SCLK_DEFAULT;

    ESP_ERROR_CHECK(uart_driver_install(GPS_UART_NUM, UART_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(GPS_UART_NUM, &gps_config));
    ESP_ERROR_CHECK(uart_set_pin(GPS_UART_NUM, BOARD_GPS_TX_PIN, BOARD_GPS_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    ESP_LOGI(TAG, "Task GPS iniciada");

    ESP_LOGI(TAG, "Task GPS iniciada");

    // ← ADICIONE ESTE BLOCO PARA TESTE
    ESP_LOGI(TAG, "🔧 TESTE: Aguardando WiFi ficar pronto...");
    int timeout = 0;
    while (!is_wifi_ready() && timeout < 100)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        timeout++;
    }

    if (is_wifi_ready())
    {
        ESP_LOGI(TAG, "✅ WiFi pronto! IP obtido. Testando envio...");
        enviarDadosGPS(-27.6, -48.5, 50.0, "teste_boot");
    }
    else
    {
        ESP_LOGE(TAG, "❌ WiFi não ficou pronto após 10 segundos!");
    }

    uint8_t *data = (uint8_t *)malloc(UART_BUF_SIZE);
    char line_buffer[LINE_BUF_SIZE];
    int line_index = 0;

    while (1)
    {
        int len = uart_read_bytes(GPS_UART_NUM, data, UART_BUF_SIZE - 1, pdMS_TO_TICKS(100));
        if (len > 0)
        {
            for (int i = 0; i < len; i++)
            {
                char c = (char)data[i];
                if (c == '\n' || c == '\r')
                {
                    if (line_index > 0)
                    {
                        line_buffer[line_index] = '\0';
                        parse_nmea_gprmc(line_buffer);
                        line_index = 0;
                    }
                }
                else if (line_index < (LINE_BUF_SIZE - 1))
                {
                    line_buffer[line_index++] = c;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    free(data);
    vTaskDelete(NULL);
}