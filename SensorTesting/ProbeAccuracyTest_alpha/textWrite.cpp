#include "textWrite.h"
#include "calcSteps.h"
#include <Arduino.h>

void calcData(int stepData[9]) { // We should really get rid of this, seems like we can just call writeSerialMonitor();
  writeSerialMonitor(&stepData[0]);
};

// Write data to serial monitor
void writeSerialMonitor(int stepData[9]) {
  for(int i = 0; i < 10; i++) {
    Serial.print(stepData[i]);
    Serial.print(" (");
    Serial.print(calcDistance(stepData[i]),4);
    Serial.print("mm)");
    Serial.print("\n");
  }
  Serial.print("\n");
  return;
};


