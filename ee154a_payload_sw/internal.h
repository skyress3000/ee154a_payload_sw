#include "telemetry.h"

// CONSTANTS
#define ADDR_INTERNAL 0x77

// FUNCTIONS
bool internal_init();
telem_point_t sample_temp_internal();  // record BME280 temperature (internal sensor)