#include "telemetry.h"
#include <stdint.h>

// CONSTANTS
#define gpsPort Serial1
#define GPS_BAUD 9600
#define LAT_BIT 0
#define LNG_BIT 1
#define ALT_BIT 2
#define SPD_BIT 3
#define TIM_BIT 4

// FUNCTIONS
bool gps_init();
void check_GPS_data();
void flightname(uint32_t* datetime);
telem_point_t sample_latitude();       // record GPS latitude
telem_point_t sample_longitude();      // record GPS longitude
telem_point_t sample_altitude();       // record GPS altitude
telem_point_t sample_speed();          // record GPS speed
telem_point_t sample_time();           // record GPS time