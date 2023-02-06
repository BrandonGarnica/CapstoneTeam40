#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Arduino.h>

#include "calcSteps.h"

#define MOTOR_STEPS 200
#define ONE_REV_8MM 8
#define ONE_STEP_40MM 40e-3

// Calculate the distance movement from the amount of steps taken
double calcDistance(int stepsTaken) {
  return stepsTaken * ONE_STEP_40MM;  // 1 step is 40um
}