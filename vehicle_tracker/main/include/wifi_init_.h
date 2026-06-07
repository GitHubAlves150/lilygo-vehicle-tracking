#ifndef WIFI_INIT_H_
#define WIFI_INIT_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void wifi_init(void);
bool is_wifi_ready(void);

#ifdef __cplusplus
}
#endif

#endif