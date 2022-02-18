#include "telemetry.h"
#include "current.h"
#include "atmosphere.h"
#include "IMU.h"
#include "GPS.h"
#include "voc.h"

#define N_LED 3
const int LEDpins[] = {9, 10, 11};

void setup() {
  Serial.begin(9600);
  
  for(int i = 0; i < N_LED; i++){
    pinMode(LEDpins[i], OUTPUT);
  }
  
  init_telemetry();

}

void loop() {
  // put your main code here, to run repeatedly:

}
