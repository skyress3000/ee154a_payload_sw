#include "telemetry.h"
#include "thermistor.h"
#include <Arduino.h>
#include <math.h>

telem_point_t sample_temp_internal(){
  // Take N_AVG readings from the thermistor, average them, and convert to a current value in amps
  telem_point_t data;
  int32_t reading = 0;

  for(int i = 0; i < N_AVG; i++){
    reading += analogRead(TEMP_PIN);
  }
  data.timestamp = millis();

  float read = reading/N_AVG;
  float voltage = read * (3.3/1023.0);
  float R = (2000 * voltage)/(1 - voltage/3.3);
  float T = pow(a1 + b1 * log(R/Rref) + c1 * pow(log(R/Rref),2) + d1 * pow(log(R/Rref),3), -1) - 273.15 - 1.32;
  data.data.data_value = T;
    
  return data;
}