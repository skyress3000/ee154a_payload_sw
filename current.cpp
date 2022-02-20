#include "telemetry.h"
#include "current.h"
#include <Arduino.h>

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

bool current_init(){
  // Set the current pin as an input
  pinMode(CURRENT_PIN, INPUT);

  Serial.println(analogRead(CURRENT_PIN));

  // Take a current reading
  telem_point_t init_reading = sample_current();
  Serial.println(init_reading.data.data_value);
  // Make sure the current is above error thershold
  if(init_reading.data.data_value > 0.03){
    return true;
  }
  else{
    return false;
  }
}