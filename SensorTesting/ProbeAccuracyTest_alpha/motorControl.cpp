#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Arduino.h>

#include "motorControl.h"
#include "BasicStepperDriver.h"

// Pins Defines
#define DIR 2
#define STEP 3
#define PROBE A0

// Motor Defines
#define MOTOR_STEPS 200
#define RPM 60
#define MICROSTEPS 1
#define FWD 1
#define BKWD -1

// Movement Defines
#define FULL_TURN MOTOR_STEPS         // 200 steps ~ 8mm
#define HALF_TURN MOTOR_STEPS / 2     // 100 steps ~ 4mm
#define QUATER_TURN MOTOR_STEPS / 4   // 50 steps ~ 2mm
#define MOVE_ONE_MM MOTOR_STEPS / 8   // 25 steps ~ 1mm
#define MOVE_ONE_STEP 1               // 1 step ~ 40um

// Variable Defines
#define NUM_OF_TESTS 10
#define TRIGGER_VAL 2.5

// Declarations
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

// Variables
int stepData [NUM_OF_TESTS] = {}; // Init data point based on the number of tests
int stepsTaken;                   // Steps taken by motor
int probeValue;                   // Curren probeValue from Arduino through a voltage divider

enum motor_States {
  init_st,        // Init SM
  reset_st,       // Reset state, inital probe of tote
  probe_st,       // Probes totes X # of times
  calculate_st,   // Calculate steps to MM & accuracy & precision
  write_st,       // Write results to txt file
} current_State;

// Standard tick function
void clockControl_tick() {

  switch (current_State) { // Transition Actions

    case init_st:
      break;

    case reset_st:
      break;

    case probe_st:
      break;

    case calculate_st:
      break;

    case write_st:
      break;

    default:
      break;
  }

  switch (current_State) { // State Actions

    case init_st:
      break;

    case reset_st:
      break;

    case probe_st:
      break;

    case calculate_st:
      break;

    case write_st:
      break;

    default:
      break;
  }
}

// Call this before you call clockControl_tick().
void clockControl_init() { current_State = init_st; }