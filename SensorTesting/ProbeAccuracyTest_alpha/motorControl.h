#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#define MOTOR_STEPS 200
// Standard tick function.
void motorControl_tick();

// Call this before you call clockControl_tick().
void motorControl_init();

#endif /* MOTORCONTROL_H */