#include "telemetry.h"
#include "current.h"
#include "atmosphere.h"
#include "IMU.h"
#include "GPS.h"
#include "voc.h"
#include "LED.h"

void setup() {
  Serial.begin(9600);
  
  pinMode(BUZZ_PIN, OUTPUT);

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
  digitalWrite(BUZZ_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZ_PIN, LOW);

  init_telemetry();

}

void loop() {
  do_telemetry_sampling();
}
