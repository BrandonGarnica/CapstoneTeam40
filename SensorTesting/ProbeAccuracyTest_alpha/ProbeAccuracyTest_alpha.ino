/*
 * Simple demo, should work with any driver board
 *
 * Connect STEP, DIR as indicated
 *
 * Copyright (C)2015-2017 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include <Arduino.h>
#include "BasicStepperDriver.h"

// Motor Defines
#define MOTOR_STEPS 200
#define RPM 60
#define MICROSTEPS 1
#define FWD 1
#define BKWD -1
#define QUATER_TURN MOTOR_STEPS / 4
#define HALF_TURN MOTOR_STEPS / 2
#define ONE_MM 25

// Pins Defines
#define DIR 2
#define STEP 3
#define PROBE A0

// Variable Defines
#define NUMBER_OF_TEST 10
#define TRIGGER_VAL 2.5

BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

int data [NUMBER_OF_TEST] = {};
int steps;
int probeValue;

void setup() {
    stepper.begin(RPM, MICROSTEPS);
}

void loop() {
  
  // Back up motor 
  stepper.move(BKWD*HALF_TURN*MICROSTEPS);

  delay(1000);

  // Move motor until probe triggers
  while (probeValue < TRIGGER_VAL) {
    probeValue = map(analogRead(PROBE), 0, 1023, 0, 5);
    // Move motor by one 1mm
    stepper.move(FWD*ONE_MM*MICROSTEPS);
    delay(500);
  }

  
  delay(1000);
    
}
