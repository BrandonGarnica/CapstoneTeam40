#include "textWrite.h"
#include <Arduino.h>


void calcData(int stepData[11]){
    //TODO Figure out way to determine the final distance to the box when the sensor trips
    writeDataToFile(&stepData[0]);

};

void writeDataToFile(int stepData[11]){
    for(int i = 1; i < 11; i++){
        Serial.print(stepData[i]);
    }
    return;
};


