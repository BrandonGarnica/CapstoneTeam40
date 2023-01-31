#include <Arduino.h>
#include "motorControl.h"

void setup() {
  // Init SM
  motorControl_init();
  Serial.begin(9600);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  // Tick SM
  motorControl_tick();
  delay(250);                      // wait for a second

  digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
  delay(250);  
}
