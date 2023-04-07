#include <Arduino.h>

#include "textWrite.h"
#include "calcSteps.h"

// Variable Defines
#define NUM_OF_TESTS 10
#define NUM_OF_DATA NUM_OF_TESTS - 1
#define INIT_VAR 0
#define SIG_FIGS 4

// Write data to serial monitor
void writeSerialMonitor(int arr[NUM_OF_DATA]) {
  for(int i = INIT_VAR; i < NUM_OF_TESTS; i++) {
    Serial.println(arr[i]);
  }
  Serial.println();
  return;
};


