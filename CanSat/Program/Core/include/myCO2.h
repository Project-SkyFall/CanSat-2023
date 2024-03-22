#ifndef MY_CO2_H
#define MY_CO2_H

#include <SensirionI2CScd4x.h>

class MyCO2 : public SensirionI2CScd4x{
    public:
    MyCO2(uint8_t address);

    byte setup(bool verbose=false);
    void getData();
    void printData();

    Status status;
    Mode mode;
    IsWorking isWorking;

    uint16_t co2;
    float temperature, humidity;

    private:
    uint8_t _address;
    bool _isDataReady;
};

extern MyCO2 scd;

#endif