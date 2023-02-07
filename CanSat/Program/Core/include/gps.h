#ifndef GPS_H
#define GPS_H

#include <SparkFun_u-blox_GNSS_Arduino_Library.h>


class MyGPS : public SFE_UBLOX_GNSS{
    public:
    byte address;
    byte status;

    bool setup(bool verbose = false);
    byte getData();
    void printData();

    long latitude;
    long longitude;
    long altitude;
    byte siv;

};

extern MyGPS gps;

#endif