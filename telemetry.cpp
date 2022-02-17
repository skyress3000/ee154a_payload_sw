#include <SPI.h>
#include <SD.h>

#include "telemetry.h"

// telemetry sampling functions
static telem_point_t sample_current();        // record battery current
static telem_point_t sample_pressure();       // record BME280 pressure
static telem_point_t sample_humidity();       // record BME280 humidity
static telem_point_t sample_temp_internal();  // record BME280 temperature (internal sensor)
static telem_point_t sample_temp_external();  // record BME280 temperature (external sensor)
static telem_point_t sample_x_accel();        // record MPU9250 x acceleration
static telem_point_t sample_y_accel();        // record MPU9250 y acceleration
static telem_point_t sample_z_accel();        // record MPU9250 z acceleration
static telem_point_t sample_x_gyro();         // record MPU9250 x gyro value
static telem_point_t sample_y_gyro();         // record MPU9250 y gyro value
static telem_point_t sample_z_gyro();         // record MPU9250 z gyro value
static telem_point_t sample_x_mag();          // record MPU9250 x magnetometer value
static telem_point_t sample_y_mag();          // record MPU9250 y magnetometer value
static telem_point_t sample_z_mag();          // record MPU9250 z magnetometer value
static telem_point_t sample_latitude();       // record GPS latitude
static telem_point_t sample_longitude();      // record GPS longitude
static telem_point_t sample_altitude();       // record GPS altitude

// list of all telemetry channels we record
telem_channel_t telem_channels[] = {
  {
    "BATTERY_CURRENT",
    "", // needs to be initialized
    sample_current,
    BAT_I_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    "BME280_PRESSURE",
    "", // needs to be initialized
    sample_pressure,
    ATMOSPHERIC_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    "BME280_HUMIDITY",
    "", // needs to be initialized
    sample_humidity,
    ATMOSPHERIC_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    "BME280_TEMP_INTERNAL",
    "", // needs to be initialized
    sample_temp_internal,
    ATMOSPHERIC_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    "BME280_TEMP_EXTERNAL",
    "", // needs to be initialized
    sample_temp_external,
    ATMOSPHERIC_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    "MPU9250_X_ACCEL",
    "", // needs to be initialized
    sample_x_accel,
    IMU_SAMPLE_RATE, // 100 Hz
    0
  },
  {
    "MPU9250_Y_ACCEL",
    "", // needs to be initialized
    sample_y_accel,
    IMU_SAMPLE_RATE, // 100 Hz
    0
  },
  {
    "MPU9250_Z_ACCEL",
    "", // needs to be initialized
    sample_z_accel,
    IMU_SAMPLE_RATE, // 100 Hz
    0
  },
  {
    "MPU9250_X_GYRO",
    "", // needs to be initialized
    sample_x_gyro,
    IMU_SAMPLE_RATE, // 100 Hz
    0
  },
  {
    "MPU9250_Y_GYRO",
    "", // needs to be initialized
    sample_y_gyro,
    IMU_SAMPLE_RATE, // 100 Hz
    0
  },
  {
    "MPU9250_Z_GYRO",
    "", // needs to be initialized
    sample_z_gyro,
    IMU_SAMPLE_RATE, // 100 Hz
    0
  },
  {
    "GPS_LAT",
    "", // needs to be initialized
    sample_latitude,
    GPS_SAMPLE_RATE, // 0.2 Hz
    0
  },
  {
    "GPS_LONG",
    "", // needs to be initialized
    sample_longitude,
    GPS_SAMPLE_RATE, // 0.2 Hz
    0
  },
  {
    "GPS_ALT",
    "", // needs to be initialized
    sample_altitude,
    GPS_SAMPLE_RATE, // 0.2 Hz
    0
  },
  {
    "GPS_SPEED",
    "", // needs to be initialized
    sample_speed,
    GPS_SAMPLE_RATE, // 0.2 Hz
    0
  },
  {
    "GPS_TIME",
    "", // needs to be initialized
    sample_time,
    GPS_SAMPLE_RATE, // 0.2 Hz
    0
  },
};

void init_telemetry(char* flight_name) {
  // concat flight name with channel name into each channel log file name
  for(int i = 0; i < N_TELEM_CHANNELS; i++) {
    // start with flight name
    strncpy(telem_channels[i].log_file_name, flight_name, 128);
    // then add _[channel name]
    strncat(telem_channels[i].log_file_name, "_", 128-strlen(telem_channels[i].log_file_name));
    strncat(telem_channels[i].log_file_name, telem_channels[i].name, 128-strlen(telem_channels[i].log_file_name));
  }

  // init sensors  
  // init interrupt?
}

void do_telemetry_sampling() {
  for(int i = 0; i < N_TELEM_CHANNELS; i++) {
    // check if it has been at least sampling period since the last sample
    // measured since last time mod period to avoid drift
    // e.g. if we have 5 ms of overhead in the measurement and wait 100ms we'd get 0ms, 105ms, 210ms, etc...
    // instead this way we get 0ms, 105ms, 205ms, etc...
    if(millis() - (telem_channels[i].last_sample % telem_channels[i].sample_period) > telem_channels[i].sample_period) {
      // time for a new sample
      telem_point_t data_point = telem_channels[i].sample_channel();
      // log it
      log_telem_point(data_point, &telem_channels[i]);
    }
  }
}

// store a single datapoint on the SD card, in the log file for the specified channel
// timestamp is stored as a 32-bit value followed by 32-bit float data point
void log_telem_point(telem_point_t data, telem_channel_t* channel) {
  File logfile = SD.open(channel->log_file_name, FILE_WRITE);
  // telemetry data and timestamp into bytes
  uint8_t data_buf[4];
  uint8_t timestamp_buf[4];
  for(int i = 0; i < 4; i++) {
    // get ith byte of both
    data_buf[i] = (data.data.data_bits >> (8*i)) & 0xFF;
    timestamp_buf[i] = (data.timestamp >> (8*i)) & 0xFF;
  }
  // write data to logfile
  logfile.write(timestamp_buf, 4);
  logfile.write(data_buf, 4);
  logfile.close();
}