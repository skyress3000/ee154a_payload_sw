#include "telemetry.h"
#include "atmosphere.h"
#include <Wire.h>
#include "SparkFunBME280.h"

BME280 internalSensor;
BME280 externalSensor;

bool atmosphere_init(){
  // Initialize the internal BME 280 (for battery temps) and external BME 280 (atmospheric measurements)
  Wire.begin();

  externalSensor.setI2CAddress(ADDR_EXTERNAL);
  bool ext_success = externalSensor.beginI2C();
  internalSensor.setI2CAddress(ADDR_INTERNAL);
  bool int_success = internalSensor.beginI2C();

  // Return true only if the external AND internal sensors are successfully initialized
  return (ext_success && int_success);
}

telem_point_t sample_pressure(){
  // record BME280 pressure from the external sensor
  telem_point_t data;

  data.data.data_value = externalSensor.readFloatPressure();
  data.timestamp = millis();

  return data;
}

telem_point_t sample_humidity(){
  // record BME280 relative humidity from the external sensor
  telem_point_t data;

  data.data.data_value = externalSensor.readFloatHumidity();
  data.timestamp = millis();

  return data;
}

telem_point_t sample_temp_external(){
  // record BME280 temperature (C) from the external sensor
  telem_point_t data;

  data.data.data_value = externalSensor.readTempC();
  data.timestamp = millis();

  return data;
}

telem_point_t sample_temp_internal(){
  // record BME280 temperature (C) from the internal sensor by the battery
  telem_point_t data;

  data.data.data_value = internalSensor.readTempC();
  data.timestamp = millis();

  return data;
}