#ifndef MY_INA_H
#define MY_INA_H

#include <Adafruit_INA219.h>

class MyINA : public Adafruit_INA219{
    public:
    MyINA(byte address);
    bool setup(bool verbose=false);
    void getData();
    void printData();

    Status status;
    Mode mode = Mode::mode_RUN;
    IsWorking isWorking = IsWorking::isWorking_FALSE;
    
    float current;
    float power;
    float voltage;

    private:
    //bool begin(uint8_t address, TwoWire *theWire = &Wire);
    //Adafruit_I2CDevice *i2c_dev = NULL;
    byte _address;
};

extern MyINA ina;

#endif