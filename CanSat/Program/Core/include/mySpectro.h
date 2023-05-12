#ifndef MY_SPECTRO_H
#define MY_SPECTRO_H

#include "globalVars.h"

#include <SparkFun_AS7265X.h>

class MySpectro : public AS7265X{
    public:
    MySpectro(TwoWire &bus = Wire);
    bool setup(bool verbose = false);
    void getData();
    void printData();

    Status status;
    IsWorking isWorking;
    Mode mode;

    bool isMeasuring;
    float data[18];

    private:
    TwoWire &_bus;
    uint8_t _address = 0x49;
};

extern MySpectro asx;

#endif