#include "telemetry.h"
#include "GPS.h"
#include <TinyGPS++.h>
#include <stdint.h>

TinyGPSPlus tinyGPS;
uint8_t sampleFlags;

bool gps_init(){
  return true;
  // Initialize the GPS over the gpsPort
  gpsPort.begin(GPS_BAUD);

  uint32_t startime = millis();
  // Wait for it to be available
  while (!gpsPort.available()){}


  // Read from the serial port once it is available until we have 3 or more satellites 
  tinyGPS.encode(gpsPort.read());
  // while(tinyGPS.satellites.value() < 3 || !tinyGPS.satellites.isValid()){
  //   tinyGPS.encode(gpsPort.read());
  //   Serial.println("Searching, found " + String(tinyGPS.satellites.value()) + " satellites");
  // }
  Serial.println("Not doing search. the following message is a lie");
  Serial.println("Lock achieved with " + String(tinyGPS.satellites.value()) + " satellites");  
  Serial.println("It took " + String(millis() - startime) + " ms to connect");

  // Clear sampleFlags to indicate nothing has been sampled from this reading
  sampleFlags = 0;

  return true;
}

void check_GPS_data(){
  return;
  // Poll GPS until the location data updates
  gpsPort.flush(); // Wait for current communication to clear

  // Keep polling until location, altitude, speed, and time are updated
  while(!(tinyGPS.location.isUpdated() || tinyGPS.altitude.isUpdated() || tinyGPS.speed.isUpdated() || tinyGPS.time.isUpdated())){
    if(gpsPort.available() > 0){
      tinyGPS.encode(gpsPort.read());
    }
  }

  // Reset sampleFlags to indicate nothing has been sampled from this reading
  sampleFlags = 0;

  return;
}

telem_point_t sample_latitude(){
  // Read the latitude from the GPS
  telem_point_t data;

  // Check if we have sampled the reading
  if(sampleFlags & (1<<LAT_BIT)){
    check_GPS_data(); // Update the location
  }
  
  data.data.data_value = tinyGPS.location.lat();
  data.timestamp = millis();

  sampleFlags |= (1<<LAT_BIT); // Set the bit to indicate we read the latitude

  return data;
}

telem_point_t sample_longitude(){
  // Read the longitude from the GPS
  telem_point_t data;

  // Check if we have sampled the reading
  if(sampleFlags & (1<<LNG_BIT)){
    check_GPS_data(); // Update the location
  }
  
  data.data.data_value = tinyGPS.location.lng();
  data.timestamp = millis();

  sampleFlags |= (1<<LNG_BIT); // Set the bit to indicate we read the longitude

  return data;
}

telem_point_t sample_altitude(){
  // Read the altitude from the GPS (meters)
  telem_point_t data;

  // Check if we have sampled the reading
  if(sampleFlags & (1<<ALT_BIT)){
    check_GPS_data(); // Update the altitude
  }
  
  data.data.data_value = tinyGPS.altitude.meters();
  data.timestamp = millis();

  sampleFlags |= (1<<ALT_BIT); // Set the bit to indicate we read the altitude

  return data;
}

telem_point_t sample_speed(){
  // Read the speed from the GPS (meters per second)
  telem_point_t data;

  // Check if we have sampled the reading
  if(sampleFlags & (1<<SPD_BIT)){
    check_GPS_data(); // Update the speed
  }
  
  data.data.data_value = tinyGPS.speed.mps();
  data.timestamp = millis();

  sampleFlags |= (1<<SPD_BIT); // Set the bit to indicate we read the speed

  return data;
}

telem_point_t sample_time(){
  // Read the time from the GPS (seconds)
  telem_point_t data;
  uint8_t hr, min, sec;

  // Check if we have sampled the reading
  if(sampleFlags & (1<<TIM_BIT)){
    check_GPS_data(); // Update the time
  }

  hr = tinyGPS.time.hour();
  min = tinyGPS.time.minute();
  sec = tinyGPS.time.second();

  float time = sec + 60*min + 60*60*hr;
  
  data.data.data_value = time;
  data.timestamp = millis();

  sampleFlags |= (1<<TIM_BIT); // Set the bit to indicate we read the time

  return data;
}

void flightname(uint32_t* datetime){
  // Modify array to have date and time

  check_GPS_data(); // Update the time
  sampleFlags |= (1<<TIM_BIT); // Set the bit to indicate we read the time

  // datetime[0] = tinyGPS.date.value();
  // datetime[1] = tinyGPS.time.value();
  datetime[0] = 21922;
  datetime[1] = 20470069;

  return;
}