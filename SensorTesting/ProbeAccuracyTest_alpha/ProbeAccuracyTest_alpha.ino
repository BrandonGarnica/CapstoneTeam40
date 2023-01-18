#include <Arduino.h>
#include "motorControl.h"

void setup() {
  // Init SM
  clockControl_init();
}

void loop() {
  // Tick SM
  clockControl_tick();
}
