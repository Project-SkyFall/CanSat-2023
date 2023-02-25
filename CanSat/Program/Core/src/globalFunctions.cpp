#include "globalVars.h"

bool wireCheck(byte address){
    Wire.beginTransmission(address);
    byte val = Wire.endTransmission();
    return (val == 0 ? true : false);
}

void printResult(bool input){
    input ? Serial.println("===OK=============================================\n") : Serial.println("===FAIL===========================================\n");
}