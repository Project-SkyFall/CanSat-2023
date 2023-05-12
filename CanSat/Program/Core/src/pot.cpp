#include "pot.h"

Pot::Pot(uint8_t address):
    _address(address){
}

bool Pot::setup(bool verbose){
    verbose ? Serial.println("---AD5246-----------------------------------------") : 0;

    if(!wireCheck(_address)) return false;
    Wire.beginTransmission(_address);
    Wire.write(0b01111111);
    Wire.endTransmission();

    return true;
}