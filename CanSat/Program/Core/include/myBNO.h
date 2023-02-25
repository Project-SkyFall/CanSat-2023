#ifndef MY_IMU_H
#define MY_IMU_H

#include <Adafruit_BNO055.h>

class MyIMU : public Adafruit_BNO055{
    public:
    MyIMU();
    bool setup(bool verbose=false);
    void getData();
    void printData();

    Status status;

    float roll;
    float pitch;
    float yaw;

    struct{
        float x;
        float y;
        float z;
    } accel;
};

extern MyIMU bno;

#endif