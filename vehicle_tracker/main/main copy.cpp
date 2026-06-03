#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

//static const char *TAG = "VEHICLE_TRACKER";
/*
#define BOARD_MODEM_TX_PIN 26
#define BOARD_MODEM_RX_PIN 27
#define BOARD_MODEM_PWR_PIN 4
#define MODEM_UART_NUM UART_NUM_1
*/
//init board
//#define BOARD_POWER_ON_PIN 12
//#define BOARD_RST_PIN 5

//#define BOARD_GPS_TX_PIN 21
//#define BOARD_GPS_RX_PIN 22

//#define GPS_UART_NUM UART_NUM_2
//#define UART_BUF_SIZE 1024
//#define LINE_BUF_SIZE 128

/*
typedef struct
{
    double latitude;
    double longitude;
    char fix_time[9];
    char fix_date[7];
    bool valid;
} gps_data_t;

static gps_data_t current_gps = {0.0, 0.0, "", "", false};
static SemaphoreHandle_t gps_mutex = NULL;

*/
/*
@note Cria o identificador de um Mutex. Ele funciona como um cadeado eletrônico impedindo que a tarefa do GPS altere a estrutura ao mesmo tempo que outra tarefa (como a do Modem) tente ler os dados, evitando travamentos por corrupção de memória.

*/
/*
void parse_nmea_gprmc(const char *line)
{
    // Recebe uma linha como: "$GPRMC,123519,A,4807.038,N,01131.000,E,..."
    // 1. Confirma se a string recebida começa com os cabeçalhos de dados recomendados mínimos de navegação (\(GPRMCou\)GNRMC). Se não for, ignora imediatamente.
    if (strncmp(line, "$GPRMC", 6) != 0 && strncmp(line, "$GNRMC", 6) != 0)
    {
        return;
    }

    char buf[LINE_BUF_SIZE];
    strncpy(buf, line, sizeof(buf) - 1); // Cria uma cópia local segura da linha capturada para proteger a memória original.
    buf[sizeof(buf) - 1] = '\0';

    char *tokens[12];
    int token_count = 0;
    char *token = buf;
    char *next_token;

    // Implementa um fatiador baseado em vírgulas. Diferente de outras abordagens, esta tolera campos vazios (comum quando o GPS perde o sinal e envia sequências como , , ,), armazenando os ponteiros de texto no vetor tokens.
    while (token_count < 12)
    {

        next_token = strchr(token, ',');
        if (next_token)
        {
            *next_token = '\0';
            tokens[token_count++] = token;
            token = next_token + 1;
        }
        else
        {
            tokens[token_count++] = token;
            break;
        }
    }

    if (token_count < 10)
        return;

    char *time_str = tokens[1];
    char *status = tokens[2];
    char *lat_str = tokens[3];
    char *lat_dir = tokens[4];
    char *lon_str = tokens[5];
    char *lon_dir = tokens[6];
    char *date_str = tokens[9];

    // Verifica se o status enviado pelo satélite é igual a 'A' (Dado Ativo/Válido).
    if (status[0] == 'A' && strlen(lat_str) > 0 && strlen(lon_str) > 0)
    {
        double raw_lat = strtod(lat_str, NULL); // Converte string para decimal.
        double raw_lon = strtod(lon_str, NULL); // Converte string para decimal.

        // Executa o cálculo matemático para converter coordenadas no padrão do protocolo náutico (Graus e Minutos) para o formato padrão do Google Maps (Graus Decimais).
        int lat_degrees = (int)(raw_lat / 100);
        double lat_minutes = raw_lat - (lat_degrees * 100);
        double converted_lat = lat_degrees + (lat_minutes / 60.0);

        // Ajusta o sinal aritmético para negativo se a direção cardeal for Sul ou Oeste.
        if (lat_dir[0] == 'S')
            converted_lat = -converted_lat;

        int lon_degrees = (int)(raw_lon / 100);
        double lon_minutes = raw_lon - (lon_degrees * 100);
        double converted_lon = lon_degrees + (lon_minutes / 60.0);
        if (lon_dir[0] == 'W')
            converted_lon = -converted_lon;

        // Tenta trancar o Mutex de proteção. Se conseguir em até 50 milissegundos, copia os novos valores para a memória global de forma segura e libera o Mutex logo em seguida com xSemaphoreGive().
        if (xSemaphoreTake(gps_mutex, pdMS_TO_TICKS(50)) == pdTRUE)
        {
            // Alimenta a estrutura gps_data_t.
            current_gps.latitude = converted_lat;
            current_gps.longitude = converted_lon;
            current_gps.valid = true;
            snprintf(current_gps.fix_time, sizeof(current_gps.fix_time), "%.6s", time_str);
            snprintf(current_gps.fix_date, sizeof(current_gps.fix_date), "%.6s", date_str);
            xSemaphoreGive(gps_mutex);

            // Exibe os dados no console com LOG nativo do ESP32.
            ESP_LOGI(TAG, "Loc: %.6f, %.6f | UTC: %s | Data: %s",
                     converted_lat, converted_lon, current_gps.fix_time, current_gps.fix_date);
        }
    }
    else
    {
        if (xSemaphoreTake(gps_mutex, pdMS_TO_TICKS(50)) == pdTRUE)
        {
            current_gps.valid = false;
            xSemaphoreGive(gps_mutex);
        }
        ESP_LOGW(TAG, "Location: INVALID (Aguardando fix satélite)");
    }
}

*/

/*
void init_board_power(void)
{
    gpio_config_t io_conf = {}; // Monta uma estrutura de controle para desativar interrupções e setar os pinos de boot elétrico da LilyGO como saídas de sinal limpas.
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = ((1ULL << BOARD_POWER_ON_PIN) | (1ULL << BOARD_RST_PIN) | (1ULL << BOARD_MODEM_PWR_PIN));
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    gpio_set_level((gpio_num_t)BOARD_POWER_ON_PIN, 1); //Envia nível lógico alto para o barramento de alimentação primário periférico da placa.
    gpio_set_level((gpio_num_t)BOARD_RST_PIN, 0);

    gpio_set_level((gpio_num_t)BOARD_MODEM_PWR_PIN, 0);// Emula via software o pressionamento do botão Power Key, segurando o pino em nível lógico alto por exatamente 1 segundo para forçar a inicialização do chipset interno de rádio do A7670G.
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level((gpio_num_t)BOARD_MODEM_PWR_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(1000));
    gpio_set_level((gpio_num_t)BOARD_MODEM_PWR_PIN, 0);

    ESP_LOGI(TAG, "Hardware energizado com sucesso.");
}
*/


/*

static void gps_task(void *pvParameters)
{
    // Configura a UART (pinos 21/22, velocidade 9600)
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

    uint8_t *data = (uint8_t *)malloc(UART_BUF_SIZE);
    char line_buffer[LINE_BUF_SIZE];
    int line_index = 0;

    while (1)
    {
        // Lê dados brutos do GPS (ex: "$GPRMC,123519,A,4807.038,N,...")
        int len = uart_read_bytes(GPS_UART_NUM, data, UART_BUF_SIZE - 1, pdMS_TO_TICKS(100));
        if (len > 0)
        {
            for (int i = 0; i < len; i++)
            {
                char c = (char)data[i];
                // Monta linhas completas (quando encontra \n)
                if (c == '\n' || c == '\r')
                {
                    if (line_index > 0)
                    {
                        line_buffer[line_index] = '\0';
                        // Quando uma linha está completa, chama parse_nmea_gprmc(line_buffer)
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
*/

/* 
static void modem_task(void *pvParameters)
{
    uart_config_t modem_config = {};
    modem_config.baud_rate = 115200;
    modem_config.data_bits = UART_DATA_8_BITS;
    modem_config.parity = UART_PARITY_DISABLE;
    modem_config.stop_bits = UART_STOP_BITS_1;
    modem_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    modem_config.source_clk = UART_SCLK_DEFAULT;

    ESP_ERROR_CHECK(uart_driver_install(MODEM_UART_NUM, UART_BUF_SIZE * 2, 0, 0, NULL, 0));
    ESP_ERROR_CHECK(uart_param_config(MODEM_UART_NUM, &modem_config));
    ESP_ERROR_CHECK(uart_set_pin(MODEM_UART_NUM, BOARD_MODEM_TX_PIN, BOARD_MODEM_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    uint8_t *data = (uint8_t *)malloc(UART_BUF_SIZE);

    while (1)
    {
        int len = uart_read_bytes(MODEM_UART_NUM, data, UART_BUF_SIZE - 1, pdMS_TO_TICKS(100));
        if (len > 0)
        {
            data[len] = '\0';
            printf("%s", (char *)data);
            fflush(stdout);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    free(data);
    vTaskDelete(NULL);
}
*/
extern "C" void app_main(void)
{
    gps_mutex = xSemaphoreCreateMutex();
    init_board_power();
    vTaskDelay(pdMS_TO_TICKS(2000));

    xTaskCreate(gps_task, "gps_task", 4096, NULL, 5, NULL);
    xTaskCreate(modem_task, "modem_task", 4096, NULL, 4, NULL);
}
