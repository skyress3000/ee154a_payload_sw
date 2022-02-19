#include <stdint.h>

#ifndef TELEMETRY_H
#define TELEMETRY_H

// FUNCTIONS

void init_telemetry();
void do_telemetry_sampling();

// CONSTANTS

// data rate shared by all atmospheric telemetry channels (in ms)
#define ATMOSPHERIC_SAMPLE_RATE 100 // 10 Hz
// data rate shared by all IMU telemetry channels (in ms)
#define IMU_SAMPLE_RATE 10 // 100 Hz
// data rate for battery current measurement (in ms)
#define BAT_I_SAMPLE_RATE 100 // 10 Hz
// data rate shared by all GPS telemetry channels (in ms)
#define GPS_SAMPLE_RATE 5000 // 0.2 Hz
// data rate for reading from VOC sensor (in ms)
#define TVOC_SAMPLE_RATE 100 // 10 Hz

// total number of telemetry channels
#define N_TELEM_CHANNELS 18

#define CS_PIN 10
#define ERR_PERIOD 500

// TYPEDEFS

// a single telemetry data point
typedef struct {
  uint32_t timestamp;   // millisecond timestamp of this datapoint
  union {               // recorded data
    float data_value;   // floating point value of the measurement
    uint32_t data_bits; // bits of the float value
  } data;
} telem_point_t;

// a telemetry channel
typedef struct {
  const char name[64];                      // channel name
  char log_file_name[128];                  // name of file to log points from this channel
  telem_point_t (*sample_channel)();        // function to record a data point from the channel
  const uint32_t sample_period;             // period to (nominally) sample this channel, in ms
  uint32_t last_sample;                     // timestamp of last sample
} telem_channel_t;

#endif