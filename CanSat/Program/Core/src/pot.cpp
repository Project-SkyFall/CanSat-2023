#include "pot.h"

Pot::Pot(uint8_t address):
    _address(address){
}

bool Pot::setup(bool verbose){
    verbose ? Serial.println("---AD5246-----------------------------------------") : 0;

    if(!wireCheck(_address)) return false;

    return true;
}