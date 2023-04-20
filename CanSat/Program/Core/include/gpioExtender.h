#ifndef GPIO_EXTENDER_H
#define GPIO_EXTENDER_H

#include "globalVars.h"
#include <PCF8574.h>

#define FPV_PIN 0
#define CAM_PIN 1

class PCA8574 : PCF8574{
    public:
    PCA8574(uint8_t address);
    bool setup(bool verbose=false);

    private:
    uint8_t _pinMode;
    uint8_t _address;
};

#endif