#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Arduino.h>

#include "BasicStepperDriver.h"
#include "motorControl.h"
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
#define QUARTER_TURN MOTOR_STEPS / 4  // 50 steps ~ 2mm
#define MOVE_ONE_MM MOTOR_STEPS / 8   // 25 steps ~ 1mm
#define MOVE_5TH_MM MOTOR_STEPS / 40  // 5 steps ~ 5th of 1mm
#define MOVE_ONE_STEP 1               // 1 step ~ 40um
#define MOVE_BY_X MOVE_ONE_STEP       // Step movement when probing

// Variable Defines
#define NUM_OF_TESTS 10
#define NUM_OF_DATA NUM_OF_TESTS - 1
#define TRIGGER_VAL 2.5
#define RESET 0
#define STEP_DATA_POS 0
#define DELAY_POST_PROBE 250
#define DELAY_PROBING 10

// SM Declaration
enum motor_States {
  init_st,        // Init SM
  wait_st,        // Wait here until BTN press
  init_probe_st,  // Inital probe of tote
  probing_st,     // Probes totes X # of times
  write_st,       // Write results to serial monitor of arduino
} current_State;

// Stepper Declaration
BasicStepperDriver stepper(MOTOR_STEPS, PIN_DIR, PIN_STEP);

// Variables
int stepsTaken;                   // Steps taken by motor
int dataEntry;                    // Keeps track of data entry #
int stepData [NUM_OF_DATA] = {};  // Init data point based on the number of tests

// Inits SM & Variables used
void motorControl_init() { 
  // Init stepper
  stepper.begin(RPM, MICROSTEPS);
  pinMode(LED_BUILTIN, OUTPUT);

  // Setting Pins as inputs
  pinMode(PIN_PROBE, INPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);
    
  // Init SM
  current_State = init_st;
}

// Returns button state
bool motorControl_buttonState() { return digitalRead(PIN_BTN); }

// Returns button state
bool motorControl_probeState() { return digitalRead(PIN_PROBE); } // Probe is set as NO (HIGH = 5V, LOW = 0V)

// Helper function for moving the motor
void motorControl_moveMotor(int direction, int numOfSteps, int microsteps = 1, int delayMS = 250) {
  stepper.move(direction*numOfSteps*microsteps);
  delay(delayMS);
}

// Standard tick function
void motorControl_tick() {

  switch (current_State) { // Transition Actions

    case init_st:
      // Transition to wait st
      current_State = wait_st;
      break;
      
    case wait_st:
      // Transition to init_probe_st when BS is low
      if (!motorControl_buttonState()) {
        // Move stepper back for initial probing
        motorControl_moveMotor(BKWD, HALF_TURN, MICROSTEPS, DELAY_POST_PROBE);
        current_State = init_probe_st;
      }
      break;

    case init_probe_st:
      // Transition when probe has been triggered
      if (motorControl_probeState()) {
        // Move stepper back to home
        motorControl_moveMotor(BKWD, FULL_TURN, MICROSTEPS, DELAY_POST_PROBE);
        current_State = probing_st;
      }
      // Increment motor forward
      motorControl_moveMotor(FWD, MOVE_BY_X, MICROSTEPS, DELAY_PROBING);
      break;

    case probing_st:
      // Transition when dateEntry has been reached
      if(dataEntry == NUM_OF_DATA) {
        // Reset steps taken
        stepsTaken = RESET;
        current_State = write_st;
      }
      // Store data when probe has been triggered
      else if (motorControl_probeState()) {
        // Store steps taken
        stepData[dataEntry++] = stepsTaken;
        // Move stepper back by amount of steps taken
        motorControl_moveMotor(BKWD, stepsTaken, MICROSTEPS, DELAY_POST_PROBE);
        stepsTaken = RESET;
      }
      else {
        // Increment steps taken
        stepsTaken += MOVE_BY_X;
        // Increment motor forward
        motorControl_moveMotor(FWD, MOVE_BY_X, MICROSTEPS, DELAY_PROBING);
      }
      break;

    case write_st:
        // Write data to serial monitor
        calcData(stepData);
        current_State = init_st;
        break;

    default:
      break;
  }

  switch (current_State) { // State Actions

    case init_st:
      // Reset variables
      dataEntry = RESET;
      stepsTaken = RESET;
      break;
      
    case wait_st:
      // Do nothing...
      break;

    case init_probe_st:
      // Do nothing...
      break;

    case probing_st:
      // Do nothing...
      break;

    case write_st:
      // Do nothing...
      break;

    default:
      break;
  }
}