#ifndef MY_IMU_H
#define MY_IMU_H

#include <Adafruit_BNO055.h>

class MyIMU : public Adafruit_BNO055{
    public:
    MyIMU(int id, uint8_t address, uint8_t reset);
    bool setup(bool verbose=false);
    void getData();
    void printData();

    Status status;
    Mode mode;
    IsWorking isWorking;

    float roll;
    float pitch;
    float yaw;

    struct{
        float x;
        float y;
        float z;
    } accel;

    private:
    uint8_t _address;
    uint8_t _reset;
};

extern MyIMU bno;

#endif