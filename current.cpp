#include "telemetry.h"
#include "current.h"
#include <Arduino.h>

bool current_init(){
  // Set the current pin as an input
  pinMode(CURRENT_PIN, INPUT);
  return true;
}

telem_point_t sample_current(){
  // Take N_AVG readings from the TIA, average them, and convert to a current value in amps
  telem_point_t data;
  int reading = 0;

  for(int i = 0; i < N_AVG; i++){
    reading += analogRead(CURRENT_PIN);
  }
  data.timestamp = millis();

  reading /= N_AVG;
  data.data.data_value = (reading - CURRENT_OFFSET)/CURRENT_GAIN;
  
  return data;
}