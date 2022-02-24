#include "telemetry.h"
#include "thermistor.h"
#include <Arduino.h>

telem_point_t sample_temp_internal(){
  // Take N_AVG readings from the thermistor, average them, and convert to a current value in amps
  telem_point_t data;
  int32_t reading = 0;

  for(int i = 0; i < N_AVG; i++){
    reading += analogRead(TEMP_PIN);
  }
  data.timestamp = millis();

  float read = reading/N_AVG;
  data.data.data_value = read;
    
  return data;
}