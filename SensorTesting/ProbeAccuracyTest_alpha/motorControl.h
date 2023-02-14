#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

// Call this before you call clockControl_tick().
void motorControl_init();

// Returns button state
bool motorControl_buttonState();

// Returns button state
bool motorControl_probeState();

// Helper function for moving the motor
void motorControl_moveMotor(int direction, int rpm, int numOfSteps, int microsteps, int delay);

// Standard tick function.
void motorControl_tick();

#endif /* MOTORCONTROL_H */