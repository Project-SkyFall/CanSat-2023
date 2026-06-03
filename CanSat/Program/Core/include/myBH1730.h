#ifndef MY_BH_H
#define MY_BH_H

#include "globalVars.h"
#include <Wire.h>
//#include <BH1730.h>

enum class BH1730_GAIN {GAIN_X1 = 0b0, GAIN_X2 = 0b1, GAIN_X64 = 0b10, GAIN_X128 = 0b11};

class MyBH1730{
    public:
    MyBH1730(BH1730_GAIN gain, TwoWire *bus = &Wire);
    bool begin();
    void setGain(BH1730_GAIN gain);
    float readLux();
    void openChannel(uint8_t address);
    void specialCommand(uint8_t command);
    bool setIntegrationCycle(uint8_t value);

    bool setup(bool verbose=false);
    void getData();
    void printData();

    Status status;
    IsWorking isWorking;
    Mode mode;

    float lightIntensity;

    private:
    uint8_t _address = 0x29;
    uint8_t _integrationCycle = 218;
    BH1730_GAIN _gain = BH1730_GAIN::GAIN_X1;
    TwoWire* _bus;
};


extern MyBH1730 bh;

#endif