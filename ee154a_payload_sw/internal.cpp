#include "telemetry.h"
#include "internal.h"
#include <Wire.h>
#include "w1SparkFunBME280.h"

w1BME280 internalSensor;

bool internal_init(){
  // Initialize the internal BME 280 (for internal temps)
  Wire1.begin();

  internalSensor.setI2CAddress(ADDR_INTERNAL);
  bool int_success = internalSensor.beginI2C();

  // Return true if it succeeds
  return int_success;
}


telem_point_t sample_temp_internal(){
  // record BME280 temperature (C) from the internal sensor
  telem_point_t data;

  data.data.data_value = internalSensor.readTempC();
  data.timestamp = millis();

  return data;
}