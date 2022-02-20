#include "telemetry.h"
#include <Arduino.h>

// CONSTANTS
#define CURRENT_PIN    A0
#define CURRENT_OFFSET 1023
#define CURRENT_GAIN   (float)(-1500)
#define N_AVG          16

// FUNCTIONS
bool current_init();
telem_point_t sample_current();