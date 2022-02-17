#include "telemetry.h"

// CONSTANTS
#define CURRENT_PIN    A0
#define CURRENT_OFFSET 1023
#define CURRENT_GAIN   -1500
#define N_AVG          16

void current_init();
telem_point_t sample_current();