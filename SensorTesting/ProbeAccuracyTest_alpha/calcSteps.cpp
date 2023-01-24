#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Arduino.h>

#include "calcSteps.h"

#define MOTOR_STEPS 200
#define ONE_REV_8MM 8
#define ONE_STEP_40UM 40e-6

// Calculate the distance movement from the amount of steps taken
double calcDistance(int stepsTaken) {
  return stepsTaken * ONE_STEP_40UM;  // 1 step is 40um
}

// Calculate & return the average from the data array
double calcAvg(double data[]) {

  double sum;
  int sizeOfArray = sizeof(data)/sizeof(data[0]); // Get the size of the array

  // Start @ pos 1, because pos 0 has init position
  for (int i = 1; i < sizeOfArray; i++) {
    sum += data[i];
  }

  return sum / sizeOfArray;
}

// Calculate & return the range from the data array
double calcRange(double data[]) {
}