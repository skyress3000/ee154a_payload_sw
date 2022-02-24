#include "telemetry.h"

#define TEMP_PIN       A2
#define N_AVG          32

telem_point_t sample_temp_internal();        // record internal temperature
