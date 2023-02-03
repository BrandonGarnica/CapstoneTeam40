#include <Arduino.h>
#include "motorControl.h"

void setup() {
  // Init SM
  motorControl_init();
  Serial.begin(9600);
}

void loop() {
  motorControl_tick();
}
