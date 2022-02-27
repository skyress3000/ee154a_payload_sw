#include "telemetry.h"
#include <Arduino.h>

// CONSTANTS
#define CURRENT_PIN    A0
#define CURRENT_OFFSET 911
#define CURRENT_GAIN   (float)(-875.481)
#define N_AVG          32

// FUNCTIONS
bool current_init();
telem_point_t sample_current();