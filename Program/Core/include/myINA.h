#ifndef MY_INA_H
#define MY_INA_H

#include <Adafruit_INA219.h>

class MyINA : public Adafruit_INA219{
    public:
    MyINA(byte address);
    bool setup(bool verbose=false);
    void getData();
    void printData();

    byte status;
    float current;
    float power;

    private:
    byte _address;
};

//extern Adafruit_INA219 ina219;
extern MyINA ina;

#endif