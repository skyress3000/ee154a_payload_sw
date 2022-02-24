#include "telemetry.h"
#include "GPS.h"
#include <TinyGPS++.h>
#include <stdint.h>

TinyGPSPlus tinyGPS;
uint8_t sampleFlags;
bool gps_disable;

bool gps_init(){
  // Initialize the GPS over the gpsPort
  gpsPort.begin(GPS_BAUD);

  // Initialize variables to check if we would time out
  uint32_t starttime =  millis();
  bool timed_out = false;

  uint32_t startime = millis();
  // Wait for it to be available
  while (!gpsPort.available()){}


  // Read from the serial port once it is available until we have 3 or more satellites or until we time out
  tinyGPS.encode(gpsPort.read());
  timed_out  = (millis() - starttime) > GPS_TIMEOUT;
  while(!timed_out && (tinyGPS.satellites.value() < 3 || !tinyGPS.satellites.isValid())){
    tinyGPS.encode(gpsPort.read());
    timed_out  = (millis() - starttime) > GPS_TIMEOUT;
    Serial.println("Searching, found " + String(tinyGPS.satellites.value()) + " satellites");
  }
  if(timed_out){ // If we timed out, disable the GPS
    gps_disable = true;
    Serial.println("GPS timeout - took too long to connect");
  }
  else{
    gps_disable = false;
    Serial.println("Lock achieved with " + String(tinyGPS.satellites.value()) + " satellites");  
    Serial.println("It took " + String(millis() - startime) + " ms to connect");
  }

  // Clear sampleFlags to indicate nothing has been sampled from this reading
  sampleFlags = 0;

  return true;
}

bool check_GPS_data(){
  // Poll GPS until the location data updates; return false if it times out
  uint32_t starttime =  millis();
  bool timed_out = false;

  if(gps_disable){ // If GPS disabled, return false (GPS data invalid)
    return false;
  }

  gpsPort.flush(); // Wait for current communication to clear

  // Keep polling until location, altitude, speed, and time are updated OR until it times out
  timed_out  = (millis() - starttime) > GPS_TIMEOUT;
  while((!timed_out) && !(tinyGPS.location.isUpdated() || tinyGPS.altitude.isUpdated() || tinyGPS.speed.isUpdated() || tinyGPS.time.isUpdated())){
    if(gpsPort.available() > 0){
      tinyGPS.encode(gpsPort.read());
    }

    timed_out  = (millis() - starttime) > GPS_TIMEOUT;
  }

  // Reset sampleFlags to indicate nothing has been sampled from this reading
  if(!timed_out){
    sampleFlags = 0;
  }
  else { // Disable the GPS if we timed out
    gps_disable = true;
  }

  return !timed_out; // Return true if we didn't time out (data is valid)
}

telem_point_t sample_latitude(){
  // Read the latitude from the GPS
  telem_point_t data;
  bool data_valid;

  // Check if we have sampled the reading
  if(sampleFlags & (1<<LAT_BIT)){
    data_valid = check_GPS_data(); // Update the location
  }
  
  data.timestamp = millis();
  if(data_valid){ // If the data was valid, store the data
    data.data.data_value = tinyGPS.location.lat();
  }
  else{ // Otherwise, store 0
    data.data.data_value = 0;
  }

  sampleFlags |= (1<<LAT_BIT); // Set the bit to indicate we read the latitude

  return data;
}

telem_point_t sample_longitude(){
  // Read the longitude from the GPS
  telem_point_t data;
  bool data_valid;

  // Check if we have sampled the reading
  if(sampleFlags & (1<<LNG_BIT)){
    data_valid = check_GPS_data(); // Update the location
  }
  
  data.timestamp = millis();
  if(data_valid){ // If the data was valid, store the data
    data.data.data_value = tinyGPS.location.lng();
  }
  else{ // Otherwise, store 0
    data.data.data_value = 0;
  }

  sampleFlags |= (1<<LNG_BIT); // Set the bit to indicate we read the latitude

  return data;
}

telem_point_t sample_altitude(){
  // Read the altitude from the GPS
  telem_point_t data;
  bool data_valid;

  // Check if we have sampled the reading
  if(sampleFlags & (1<<ALT_BIT)){
    data_valid = check_GPS_data(); // Update the location
  }
  
  data.timestamp = millis();
  if(data_valid){ // If the data was valid, store the data
    data.data.data_value = tinyGPS.altitude.meters();
  }
  else{ // Otherwise, store 0
    data.data.data_value = 0;
  }

  sampleFlags |= (1<<ALT_BIT); // Set the bit to indicate we read the latitude

  return data;
}

telem_point_t sample_speed(){
  // Read the speed from the GPS (meters per second)
  telem_point_t data;
  bool data_valid;

  // Check if we have sampled the reading
  if(sampleFlags & (1<<SPD_BIT)){
    data_valid = check_GPS_data(); // Update the location
  }
  
  data.timestamp = millis();
  if(data_valid){ // If the data was valid, store the data
    data.data.data_value = tinyGPS.speed.mps();
  }
  else{ // Otherwise, store 0
    data.data.data_value = 0;
  }

  sampleFlags |= (1<<SPD_BIT); // Set the bit to indicate we read the latitude

  return data;
}

telem_point_t sample_time(){
  // Read the time from the GPS (seconds)
  telem_point_t data;
  bool data_valid;
  uint8_t hr, min, sec;
  float time;

  // Check if we have sampled the reading
  if(sampleFlags & (1<<TIM_BIT)){
    data_valid = check_GPS_data(); // Update the time
  }

  data.timestamp = millis();

  if(data_valid){
    hr = tinyGPS.time.hour();
    min = tinyGPS.time.minute();
    sec = tinyGPS.time.second();
    time = sec + 60*min + 60*60*hr;
    data.data.data_value = time;
  }
  else{
    data.data.data_value = 0;
  }

  sampleFlags |= (1<<TIM_BIT); // Set the bit to indicate we read the time

  return data;
}

void flightname(uint32_t* datetime){
  bool data_valid;
  // Modify array to have date and time

  data_valid = check_GPS_data(); // Update the time
  sampleFlags |= (1<<TIM_BIT); // Set the bit to indicate we read the time
  
  if(data_valid){
    datetime[0] = tinyGPS.date.value();
    datetime[1] = tinyGPS.time.value();
  }
  else{
    datetime[0] = CURRENT_DATE;
    datetime[1] = CURRENT_TIME;
  }

  return;
}