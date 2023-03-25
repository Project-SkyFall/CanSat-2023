#include "globalVars.h"

bool wireCheck(byte address, TwoWire &theWire){
    theWire.beginTransmission(address);
    byte val = theWire.endTransmission();
    return (val == 0 ? true : false);
}

void printResult(bool input){
    input ? Serial.println("===OK=============================================\n") : Serial.println("===FAIL===========================================\n");
}