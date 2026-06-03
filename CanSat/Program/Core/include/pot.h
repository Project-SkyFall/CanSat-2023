#ifndef POT_H
#define POT_H

#include "globalVars.h"

class Pot {
    public:
    Pot(uint8_t address);
    bool setup(bool verbose = false);

    private:
    uint8_t _address;
};

#endif