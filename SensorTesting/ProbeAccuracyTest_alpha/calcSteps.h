#ifndef CALCSTEPS_H
#define CALCSTEPS_H

#include <stdbool.h>
#include <stdint.h>

// Calculate the distance movement from the amount of steps taken
double calcDistance (int stepsTaken);

// Calculate & return the average from the data array
double calcAvg (double data[]);

// Calculate & return the range from the data array
double calcRange (double data[]);

#endif /* CALCSTEPS_H */