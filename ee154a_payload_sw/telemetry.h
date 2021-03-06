#include <stdint.h>

#ifndef TELEMETRY_H
#define TELEMETRY_H

// FUNCTIONS

void init_telemetry();
void do_telemetry_sampling();
void stop_telemetry();

// CONSTANTS

// data rate shared by all atmospheric telemetry channels (in ms)
#define ATMOSPHERIC_SAMPLE_RATE 500 // 2 Hz
// data rate shared by all IMU telemetry channels (in ms)
#define IMU_SAMPLE_RATE 100 // 10 Hz
// data rate for battery current measurement (in ms)
#define BAT_I_SAMPLE_RATE 500 // 2 Hz
// data rate shared by all GPS telemetry channels (in ms)
#define GPS_SAMPLE_RATE 1000 // 1 Hz
// data rate for reading from VOC sensor (in ms)
#define TVOC_SAMPLE_RATE 500 // 2 Hz

// total number of telemetry channels
#define N_TELEM_CHANNELS 22

#define CS_PIN 10
#define NEW_FILE_PERIOD 600000 // Make a new file every 10 minutes
#define ERR_PERIOD 500 // LED blink period

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
  const uint8_t log_flag;                   // flag indicating which sensor read this data
  telem_point_t (*sample_channel)();        // function to record a data point from the channel
  const uint32_t sample_period;             // period to (nominally) sample this channel, in ms
  uint32_t last_sample;                     // timestamp of last sample
} telem_channel_t;

#endif