#include "telemetry.h"

void setup() {
  Serial.begin(9600);
  
  // ADD: any initial startup code

  pinMode(STOP_PIN, INPUT_PULLUP); // Set internal pullup resistor - no extra parts!
 
  init_telemetry();

  // ADD: any indication of startup complete
}

void loop() {
  do_telemetry_sampling();

  if(!digitalRead(STOP_PIN)){ // When we see the STOP_PIN pulled down, stop operation
    stop_telemetry();
  }
}
