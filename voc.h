#include "telemetry.h"

// CONSTANTS
#define eCO2_BASELINE 0 // Hard-coded calibration values
#define TVOC_BASELINE 0
// FUNCTIONS
bool tvoc_init();
telem_point_t sample_tvoc();        // record Total Volatile Organic Compound
telem_point_t sample_eco2();        // record equivalent CO2