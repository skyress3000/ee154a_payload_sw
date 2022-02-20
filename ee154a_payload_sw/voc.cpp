#include <Wire.h>
#include "Adafruit_SGP30.h"
#include "voc.h"

Adafruit_SGP30 sgp;

bool tvoc_init(){
  // Initialize the SGP30 and set baseline values
  bool success = sgp.begin();

  // Return false if we didn't successfully initialize
  if (!success){
    return false;
  }
  // Otherwise, keep initializing the sensor with hard-coded calibration values
  sgp.setIAQBaseline(eCO2_BASELINE, TVOC_BASELINE);

  return true;
}

telem_point_t sample_tvoc(){
  // record SGP30 VOC data
  telem_point_t data;
  sgp.IAQmeasure();

  data.data.data_value = sgp.TVOC;
  data.timestamp = millis();

  return data;
}

telem_point_t sample_eco2(){
  // record SGP30 equivalent CO2 data
  telem_point_t data;
  sgp.IAQmeasure();

  data.data.data_value = sgp.eCO2;
  data.timestamp = millis();

  return data;
}