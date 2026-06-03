#ifndef GPS_PARSER_H
#define GPS_PARSER_H

#include <stdbool.h>

typedef struct {
    double latitude;
    double longitude;
    char fix_time[9];
    char fix_date[7];
    bool valid;
} gps_data_t;

#ifdef __cplusplus
extern "C" {
#endif

void gps_task(void *pvParameters);



#ifdef __cplusplus
}
#endif

#endif // GPS_PARSER_H