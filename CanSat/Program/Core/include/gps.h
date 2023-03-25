#ifndef GPS_H
#define GPS_H

#include <SparkFun_u-blox_GNSS_Arduino_Library.h>


class MyGPS : public SFE_UBLOX_GNSS{
    public:
    MyGPS(byte address);

    Status status = Status::status_NACK;
    Mode mode = Mode::mode_RUN;
    IsWorking isWorking = IsWorking::isWorking_FALSE;

    bool setup(bool verbose = false);
    void getData();
    void printData();
    

    long latitude;
    long longitude;
    long altitude;
    byte siv;

    private:
    byte _address;
};

extern MyGPS gps;

#endif