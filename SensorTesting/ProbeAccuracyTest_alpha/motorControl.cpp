#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Arduino.h>

#include "motorControl.h"
#include "BasicStepperDriver.h"
#include "textWrite.h"

// Pins Defines
#define PIN_DIR 2
#define PIN_STEP 3
#define PIN_PROBE 4
#define PIN_BTN 5

// Motor Defines
#define MOTOR_STEPS 200
#define RPM 60
#define MICROSTEPS 1
#define FWD 1
#define BKWD -1

// Movement Defines
#define FULL_TURN MOTOR_STEPS         // 200 steps ~ 8mm
#define HALF_TURN MOTOR_STEPS / 2     // 100 steps ~ 4mm
#define QUARTER_TURN MOTOR_STEPS / 4   // 50 steps ~ 2mm
#define MOVE_ONE_MM MOTOR_STEPS / 8   // 25 steps ~ 1mm
#define MOVE_ONE_STEP 1               // 1 step ~ 40um

// Variable Defines
#define NUM_OF_TESTS 10
#define NUM_OF_DATA NUM_OF_TESTS + 1
#define TRIGGER_VAL 2.5
#define RESET 0
#define STEP_DATA_POS 0

// Declarations

int stepsTaken;                                             // Steps taken by motor
int dataEntry;
int probeValue;
int buttonState = 0;

BasicStepperDriver stepper(MOTOR_STEPS, PIN_DIR, PIN_STEP);

// Variables
int stepData [NUM_OF_DATA] = {}; // Init data point based on the number of tests

enum motor_States {
  init_st,        // Init SM
  wait_st,        // Wait here until BTN press
  init_probe_st,  // Inital probe of tote
  probing_st,     // Probes totes X # of times
  calculate_st,   // Calculate steps to MM & accuracy & precision
  write_st,       // Write results to txt file
} current_State;

// Standard tick function
void motorControl_tick() {
  
  probeValue = map(analogRead(PIN_PROBE), 0, 1023, 0, 5); // Map the probe analogue value to a 0 - 5
  buttonState = digitalRead(PIN_BTN);
  switch (current_State) { // Transition Actions

    case init_st:
      // Transition to wait st
      current_State = wait_st;
      break;
      
    case wait_st:
      // Transition to init_probe_st when BTN is pressed
      if (buttonState == LOW) {
        // Move stepper back to home
        stepper.move(BKWD*FULL_TURN*MICROSTEPS);
        current_State = init_probe_st;
      }
      break;

    case init_probe_st:
      // Transition when probe has been triggered
      if (probeValue >= TRIGGER_VAL) {
        // Store steps taken
        stepData[dataEntry++] = stepsTaken;
        stepsTaken = RESET;
        current_State = probing_st;
      }
      // Increment steps by one mm
      stepsTaken += MOVE_ONE_MM;
      // Increment motor forward by one 1mm
      stepper.move(FWD*MOVE_ONE_MM*MICROSTEPS);
      break;

    case probing_st:
      // Transition when dateEntry has been reached
      if(dataEntry == NUM_OF_DATA) {
        stepsTaken = RESET;
        // Move stepper back
        stepper.move(BKWD*FULL_TURN*MICROSTEPS);
        current_State = calculate_st;
      }
      // Store data when probe has been triggered
      else if (probeValue >= TRIGGER_VAL) {
        // Store steps taken
        stepData[dataEntry++] = stepsTaken;
        stepsTaken = RESET;
        // Move stepper back
        stepper.move(BKWD*FULL_TURN*MICROSTEPS);
      }
      else {
        // Increment steps by one mm
        stepsTaken += MOVE_ONE_MM;
        // Increment motor forward by one 1mm
        stepper.move(FWD*MOVE_ONE_MM*MICROSTEPS);
      }
      break;

    case calculate_st:
        calcData()
        break;

    case write_st:
      break;

    default:
      break;
  }

  switch (current_State) { // State Actions

    case init_st:
      // Reset variables
      dataEntry = RESET;
      stepsTaken = RESET;
      probeValue = RESET;
      break;
      
    case wait_st:
      break;

    case init_probe_st:

      break;

    case probing_st:
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
void motorControl_init() { 
  stepper.begin(RPM, MICROSTEPS);
  pinMode(LED_BUILTIN, OUTPUT);
  // Setting Pins as inputs
  pinMode(PIN_PROBE, INPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);
    
  // Init SM
  current_State = init_st;
}