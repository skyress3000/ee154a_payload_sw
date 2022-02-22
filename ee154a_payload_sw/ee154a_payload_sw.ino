#include "telemetry.h"
#include "current.h"
#include "atmosphere.h"
#include "IMU.h"
#include "GPS.h"
#include "voc.h"
#include "PINS.h"

void setup() {
  Serial.begin(9600);
  
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(STOP_PIN, INPUT_PULLUP); // Set internal pullup resistor - no extra parts!

  for(int i = 0; i < N_LED; i++){
    pinMode(LEDpins[i], OUTPUT);
  }
  for(int i = 0; i < N_LED; i++){
    digitalWrite(LEDpins[i], HIGH);
    delay(500);
  }
  for(int i = 0; i < N_LED; i++){
    digitalWrite(LEDpins[i], LOW);
    delay(500);
  }
 
  init_telemetry();

  digitalWrite(BUZZ_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZ_PIN, LOW);
}

void loop() {
  do_telemetry_sampling();

  if(!digitalRead(STOP_PIN)){ // When we see the STOP_PIN pulled down, stop operation and blink the LEDs
    stop_telemetry();
  }
}
