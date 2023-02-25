#include "globalVars.h"

#include "myCO2.h"

MyCO2::MyCO2(uint8_t address):
    _address(address)
{}

byte MyCO2::setup(bool verbose){
    verbose ? Serial.println("---SCD41 setup------------------------------------") : 0;

    scd.begin(Wire);

    return true;
}

void MyCO2::getData(){

}

void MyCO2::printData(){

}