#include "textWrite.h"
#include <iostream>
#include <fstream>

void calcData(int stepData[11]){
    //TODO Figure out way to determine the final distance to the box when the sensor trips
    writeDataToFile();

};

void writeDataToFile(){
    std::ofstream dataFile;
    dataFile.open ("sensorData.txt");
    dataFile << "Testing writing to file\n";
    dataFile.close();
    return;
};


