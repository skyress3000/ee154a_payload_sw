#include <SPI.h>
#include <SD.h>
#include <stdint.h>

#include "telemetry.h"
#include "GPS.h"
#include "current.h"
#include "atmosphere.h"
#include "IMU.h"
#include "voc.h"
#include "PINS.h"

File logfile;

// list of all telemetry channels we record
telem_channel_t telem_channels[] = {
  {
    'B', // B for Battery current
    sample_current,
    BAT_I_SAMPLE_RATE, // 0.2 Hz
    0
  },
  {
    'P', // Pressure
    sample_pressure,
    ATMOSPHERIC_SAMPLE_RATE, // 0.2 Hz
    0
  },
  {
    'H', // Humidity
    sample_humidity,
    ATMOSPHERIC_SAMPLE_RATE, // 0.2 Hz
    0
  },
  {
    't', // Internal temperature
    sample_temp_internal,
    ATMOSPHERIC_SAMPLE_RATE, // 0.2 Hz
    0
  },
  {
    'T', // External Temperature
    sample_temp_external,
    ATMOSPHERIC_SAMPLE_RATE, // 0.2 Hz
    0
  },
  {
    'i', // X acceleration
    sample_x_accel,
    IMU_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    'j', // y acceleration
    sample_y_accel,
    IMU_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    'k', // z acceleration
    sample_z_accel,
    IMU_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    'l', // x gyro
    sample_x_gyro,
    IMU_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    'm', // y gyro
    sample_y_gyro,
    IMU_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    'o', // z gyro
    sample_z_gyro,
    IMU_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    'p', // x magnetometer
    sample_x_mag,
    IMU_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    'q', // y magnetometer
    sample_y_mag,
    IMU_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    'r', // z magnetometer
    sample_z_mag,
    IMU_SAMPLE_RATE, // 10 Hz
    0
  },
  {
    'L', // Latitude
    sample_latitude,
    GPS_SAMPLE_RATE, // 0.1 Hz
    0
  },
  {
    'l', // Longitude
    sample_longitude,
    GPS_SAMPLE_RATE, // 0.1 Hz
    0
  },
  {
    'A', // Altitude
    sample_altitude,
    GPS_SAMPLE_RATE, // 0.1 Hz
    0
  },
  {
    'S', // Speed
    sample_speed,
    GPS_SAMPLE_RATE, // 0.1 Hz
    0
  },
  {
    'I', // GPS Time
    sample_time,
    GPS_SAMPLE_RATE, // 0.1 Hz
    0
  },
  {
    'V', // VOC
    sample_tvoc,
    TVOC_SAMPLE_RATE, // 0.2 Hz
    0
  },
    {
    'C', // eCO2
    sample_eco2,
    TVOC_SAMPLE_RATE, // 0.2 Hz
    0
  },
};

void init_telemetry() {
  bool success, all_success;
  all_success = true;

  Serial.println("Init");

  success = current_init();
  all_success &= success;
  Serial.println("Current Sensor: " + String(success));

  success = atmosphere_init();
  all_success &= success;
  Serial.println("BME280 sensors: " + String(success)); 

  success = imu_init();
  all_success &= success;
  Serial.println("MPU9250_DMP: " + String(success)); 

  success = tvoc_init();
  all_success &= success;
  Serial.println("TVOC: " + String(success));

  if(all_success){
    digitalWrite(LEDpins[0], HIGH); // indicate the sensors initialized
  }
  else{
    while(true){ // Blink the LED to indicate failure
      digitalWrite(LEDpins[0], HIGH);
      delay(ERR_PERIOD);
      digitalWrite(LEDpins[0], LOW);
      delay(ERR_PERIOD);
    }
  }

  success = gps_init();
  all_success &= success;
  Serial.println("GPS: " + String(success));

  if(all_success){
    digitalWrite(LEDpins[1], HIGH); // indicate the GPS initialized
  }
  else{
    while(true){ // Blink the LED to indicate failure
      digitalWrite(LEDpins[1], HIGH);
      delay(ERR_PERIOD);
      digitalWrite(LEDpins[1], LOW);
      delay(ERR_PERIOD);
    }
  }

  // Initialize SD card
  success = SD.begin(CS_PIN);
  all_success &= success;
  Serial.println("SD Card: " + String(success));
  
  char flight_name[128];
  uint32_t datetime[2];
  flightname(datetime);

  sprintf(flight_name, "%"PRIu32"/%"PRIu32 , datetime[0], datetime[1]);

  // concat flight name with channel name into log file name
  strncat(flight_name, "/LOG", 128-strlen(flight_name));
  Serial.println(flight_name);
  SD.mkdir(flight_name);

  
  if(all_success){
    digitalWrite(LEDpins[2], HIGH); // indicate the SD card initialized
  }
  else{
    while(true){ // Blink the LED to indicate failure
      digitalWrite(LEDpins[2], HIGH);
      delay(ERR_PERIOD);
      digitalWrite(LEDpins[2], LOW);
      delay(ERR_PERIOD);
    }
  }

  // Open the file and keep it open
  logfile = SD.open(flight_name, O_CREAT | O_WRITE);
}

// store a single datapoint on the SD card, in the log file with the identity byte indicating which data was read
// timestamp is stored as a 32-bit value followed by 32-bit float data point
static void log_telem_point(telem_point_t data, uint8_t identity) {
  // telemetry data and timestamp into bytes
  uint8_t data_buf[4];
  uint8_t timestamp_buf[4];
  for(int i = 0; i < 4; i++) {
    // get ith byte of both
    data_buf[i] = (uint8_t) (data.data.data_bits >> (8*i)) & 0xFF;
    timestamp_buf[i] = (data.timestamp >> (8*i)) & 0xFF;
  }
  // write data to logfile
  logfile.write(identity);
  logfile.write(timestamp_buf, 4);
  logfile.write(data_buf, 4);
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
      log_telem_point(data_point, telem_channels[i].log_flag);
    }
  }
}

void stop_telemetry() {
  logfile.close();
  while(1){
    for(int i = 0; i < N_LED; i++){
      digitalWrite(LEDpins[i], HIGH);
      delay(500);
    }
    for(int i = 0; i < N_LED; i++){
      digitalWrite(LEDpins[i], LOW);
      delay(500);
    }
  }
}