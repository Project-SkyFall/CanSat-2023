#ifndef OXYGEN_H
#define OXYGEN_H

#include "globalVars.h"

class MyOxygen{
    public: 
    MyOxygen(uint8_t pin);
    bool setup(bool verbose=false);
    void getData();
    void printData();

    Status status;
    float concentration;

    private:
    uint8_t _pin;
};

extern MyOxygen oxygen;

#endif