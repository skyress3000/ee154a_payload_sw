#include "telemetry.h"

// CONSTANTS
#define ADDR_INTERNAL 0x76
#define ADDR_EXTERNAL 0x77

// FUNCTIONS
bool atmosphere_init();
telem_point_t sample_pressure();       // record BME280 pressure
telem_point_t sample_humidity();       // record BME280 humidity
telem_point_t sample_temp_bat();  // record BME280 temperature (internal sensor)
telem_point_t sample_temp_external();  // record BME280 temperature (external sensor)