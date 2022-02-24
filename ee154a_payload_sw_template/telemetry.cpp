#include <SPI.h>
#include <SD.h>
#include <stdint.h>

#include "telemetry.h"
// ADD: import any sensor sampling functions

File logfile; // File object and filename are shared variables
char flight_name[128]; 
int idx = 0;
uint32_t last_filetime;

// list of all telemetry channels we record
telem_channel_t telem_channels[] = {
  // ADD: each of your telemetry channels with a unique ID (arbitrary byte), function to sample this channel (should return telem_point_t), data rate in ms, and last sample time initialized to 0
  /* EXAMPLE:
  { // battery current
    BAT_CURRENT_ID, // identifier
    sample_current, // sample function
    BAT_I_SAMPLE_RATE, // data rate
    0 // last sample time
  },
  { // pressure
    PRESSURE_ID, // identifier
    sample_pressure, // sample function
    ATMOSPHERIC_SAMPLE_RATE, // data rate
    0 // last sample time
  },
  */
};

void init_telemetry() {
  bool success;

  // ADD: initialize your sensors here and verify it worked

  // Initialize SD card
  success = SD.begin(CS_PIN);
  
  // CHANGE: the flight name is currently extracted from the GPS date+time, you can either copy-paste our function or change this if you want
  // I didn't include the flightname() function in the template because it's somewhat specific to our GPS implementation
  // but it's in the non-template folder if you want it
  uint32_t datetime[2];
  flightname(datetime);

  sprintf(flight_name, "%"PRIu32"/%"PRIu32 , datetime[0], datetime[1]);
  SD.mkdir(flight_name);
  // Create the new filename
  char filename[128];
  // concat flight name with channel name into log file name
  sprintf(filename, "%s/LOG%d", flight_name, idx);
  Serial.println(filename);
  last_filetime = millis();

  // Open the file and keep it open
  logfile = SD.open(filename, O_CREAT | O_WRITE);

  // ADD: handle any failures in initialization
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

void renew_file(){
  // Close the current file and open a new one with an incremented file name 
  logfile.close();
  idx++;
  // Create the new filename
  char filename[128];
  // concat flight name with channel name into log file name
  sprintf(filename, "%s/LOG%d", flight_name, idx);
  // Open the new file
  logfile = SD.open(filename, O_CREAT | O_WRITE);
}

void do_telemetry_sampling() {
  // Check if it's time to make a new file
  uint32_t file_time = last_filetime - (last_filetime % NEW_FILE_PERIOD);
  if(millis() - file_time > NEW_FILE_PERIOD) {
    last_filetime = millis();
    renew_file();
  }

  for(int i = 0; i < N_TELEM_CHANNELS; i++) {
    // check if it has been at least sampling period since the last sample
    // measured since last multiple of period to avoid drift
    // e.g. if we have 5 ms of overhead in the measurement and wait 100ms we'd get 0ms, 105ms, 210ms, etc...
    // instead this way we get 0ms, 105ms, 205ms, etc...
    uint32_t corrected_time = telem_channels[i].last_sample - (telem_channels[i].last_sample % telem_channels[i].sample_period);
    if(millis() - corrected_time > telem_channels[i].sample_period) {
      // update last sample 
      telem_channels[i].last_sample = millis();
      // time for a new sample
      telem_point_t data_point = telem_channels[i].sample_channel();
      // log it
      log_telem_point(data_point, telem_channels[i].log_flag);
    }
  }
}

void stop_telemetry() {
  logfile.close();
  
  // ADD: indication that telemetry gathering has stopped
}