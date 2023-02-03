#include <Arduino.h>

#include "textWrite.h"
#include "calcSteps.h"

// Variable Defines
#define NUM_OF_TESTS 10
#define NUM_OF_DATA NUM_OF_TESTS - 1
#define INIT_VAR 0
#define SIG_FIGS 4

void calcData(int stepData[NUM_OF_DATA]) { // We should really get rid of this, seems like we can just call writeSerialMonitor();
  writeSerialMonitor(&stepData[INIT_VAR]);
};

// Write data to serial monitor
void writeSerialMonitor(int stepData[NUM_OF_DATA]) {
  for(int i = INIT_VAR; i < NUM_OF_TESTS; i++) {
    Serial.print(stepData[i]);
    Serial.print(" (");
    Serial.print(calcDistance(stepData[i]), SIG_FIGS);
    Serial.print("mm)");
    Serial.print("\n");
  }
  Serial.print("\n");
  return;
};


