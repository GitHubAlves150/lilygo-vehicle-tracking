#ifndef WIFI_H
#define WIFI_H

#include <stdbool.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h" 
#ifdef __cplusplus
extern "C"
{
#endif

    void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    void wifi_init(void);

#ifdef __cplusplus
}
#endif

#endif // WIFI_H
