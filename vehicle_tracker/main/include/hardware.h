#ifndef HARDWARE_H
#define HARDWARE_H

#include <stdint.h>
#include "driver/uart.h"

// Definições de Hardware do LilyGO T-A7670
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

#ifdef __cplusplus
extern "C" {
#endif

void init_board_power(void);

#ifdef __cplusplus
}
#endif

#endif // HARDWARE_H