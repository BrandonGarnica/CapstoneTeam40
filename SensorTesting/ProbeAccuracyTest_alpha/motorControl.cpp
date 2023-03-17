#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Arduino.h>

#include "BasicStepperDriver.h"
#include "motorControl.h"
#include "textWrite.h"

// Arduino Pins Defines
#define PIN_DIR 2
#define PIN_STEP 3
#define PIN_PROBE 0
#define PIN_BTN 5

// Motor Defines
#define MOTOR_STEPS 200
#define RPM_PROBING 30
#define RPM_MOVING 60
#define RPM_DEFAULT 30
#define RPM_RESET 60
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
#define NUM_OF_DATA NUM_OF_TESTS
#define TRIGGER_VAL 2.5
#define RESET 0
#define STEP_DATA_POS 0
#define DELAY_DEFAULT 250
#define DELAY_POST_PROBE 10
#define DELAY_PROBING 25
#define MIN_PROBE_DISTANCE 100
#define ANALOG_VOLT_100MM 0.681

// SM Declaration
enum motor_States {
  init_st,        // Init SM
  wait_st,        // Wait here until BTN press
  backup_st,      // Inital probe of tote
  forward_st,     // Probes totes X # of times
  write_st,       // Write results to serial monitor of arduino
} current_State;

// Stepper Declaration
BasicStepperDriver stepper(MOTOR_STEPS, PIN_DIR, PIN_STEP);

// Variables
bool firstProbe = true;           // Variable to track if its the first probe of the test

int stepsTakenFWD;                // Steps taken FWD by motor
int stepsTakenBKWD;               // Steps taken BKWD by motor
int sensorLowPosition;
int sensorHighPosition;
int dataEntry;                    // Keeps track of data entry #
int stepData [NUM_OF_DATA] = {};  // Init data point based on the number of tests

float range;
float lowVolt = 0.07;
float highVolt = 4.467;
float lowAnalog = 20.000;
float highAnalog = 970.0;

// Inits SM & Variables used
void motorControl_init() { 
  // Init stepper
  stepper.begin(RPM_DEFAULT, MICROSTEPS);
  pinMode(LED_BUILTIN, OUTPUT);

  // Init PINS on arduino
  pinMode(PIN_PROBE, INPUT);
  pinMode(PIN_BTN, INPUT_PULLUP);
    
  // Init ultrasonic sensor range
  range = (highVolt - lowVolt) / (highAnalog - lowAnalog);
  
  // Init SM
  current_State = init_st;
}

// Maps the input analog to a voltage output
float motorControl_mapInput(float analogInput) {
  return (analogInput - lowAnalog) * range + lowVolt;
}

// Returns button state
bool motorControl_buttonState() { return digitalRead(PIN_BTN); }

// Returns probe state
// bool motorControl_probeState() { return digitalRead(PIN_PROBE); } // Probe is set as NO (HIGH = 5V, LOW = 0V)
bool motorControl_probeState() { return motorControl_mapInput(analogRead(0)) >= ANALOG_VOLT_100MM ? 1:0; } // Probe returns high when analog is above min

// Helper function for moving stepper motor
void motorControl_moveMotor(int direction, int numOfSteps, int rpm = RPM_MOVING, int microsteps = MICROSTEPS, int delayMS = DELAY_DEFAULT) {
  stepper.begin(rpm, microsteps);
  stepper.move(direction*numOfSteps*microsteps);
  delay(delayMS);
}

// Motor Control tick function
void motorControl_tick() {

  switch (current_State) { // Transition Actions

    case init_st:
      // Transition to wait st
      current_State = wait_st;
      break;
      
    case wait_st:
      // Transition to when BNT is LOW
      if (!motorControl_buttonState()) {
        current_State = backup_st;
      }
      break;

    case backup_st:
      // Need to obtain position from box on the first probe
      if(firstProbe) {
        // Transition when probe is HIGH
        if (motorControl_probeState()) {
          firstProbe = false;   // No longer the first probe
          delay(500);           // Waiting for signal to settle
          current_State = forward_st;
        }
        sensorLowPosition += MOVE_BY_X;
        // Increment motor BKWDs
        motorControl_moveMotor(BKWD, MOVE_BY_X, RPM_PROBING, MICROSTEPS, DELAY_PROBING);
      }
      // When it's not the first probe...
      if(!firstProbe) {
        if(motorControl_probeState() && (stepsTakenBKWD >= sensorHighPosition)){
          motorControl_moveMotor(FWD, (stepsTakenBKWD - sensorHighPosition), RPM_PROBING, MICROSTEPS, DELAY_PROBING);
          delay(500);   // Wait for signal to settle
          sensorHighPosition = RESET;
          stepsTakenBKWD = RESET;
          current_State = forward_st;
        } else {
          stepsTakenBKWD += MOVE_BY_X;
          // Increment motor BKWDs
          motorControl_moveMotor(BKWD, MOVE_BY_X, RPM_PROBING, MICROSTEPS, DELAY_PROBING);
        }
      }
      break;

    case forward_st:
      // Transition when # of test has been reached
      if(dataEntry >= NUM_OF_TESTS) {
        // Store data
        stepData[dataEntry++] = sensorLowPosition - sensorHighPosition;
        // Move stepper back by amount of steps taken back in first probe
        motorControl_moveMotor(FWD, sensorLowPosition, RPM_RESET,  MICROSTEPS, DELAY_POST_PROBE);
        current_State = write_st;
      }
      // Store data when probe has been triggered
      else if (!motorControl_probeState()) {
        // Store diff. between first probe and current probe
        stepData[dataEntry++] = sensorLowPosition - sensorHighPosition;
        delay(1000);   // Wait for signal to settle
        current_State = backup_st;
      }
      else {
        sensorHighPosition += MOVE_BY_X;
        // Increment motor forward
        motorControl_moveMotor(FWD, MOVE_BY_X, RPM_MOVING, MICROSTEPS, DELAY_POST_PROBE);
      }
      break;

    case write_st:
      // Write data to serial monitor
      writeSerialMonitor(stepData);
      current_State = init_st;
      break;

    default:
      break;
  }

  switch (current_State) { // State Actions

    case init_st:
      // Reset variables
      dataEntry = RESET;
      stepsTakenFWD = RESET;
      stepsTakenBKWD = RESET;
      sensorHighPosition = RESET;
      sensorLowPosition = RESET;
      firstProbe = true;
      break;
      
    case wait_st:
      // Do nothing...
      break;

    case backup_st:
      // Do nothing...
      break;

    case forward_st:
      // Increment steps taken
      stepsTakenFWD += MOVE_BY_X;
      break;

    case write_st:
      // Reset steps taken for next test
      stepsTakenFWD = RESET;
      break;

    default:
      break;
  }
}