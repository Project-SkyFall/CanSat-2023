#include "gpioExtender.h"

PCA8574::PCA8574(uint8_t address):
    _address(address){
}

bool PCA8574::setup(bool verbose){
    verbose ? Serial.println("---PCA8574 setup----------------------------------") : 0;

    if(!wireCheck(_address)) return false;

    return true;
}