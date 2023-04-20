#ifndef GPS_H
#define GPS_H

#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "globalVars.h"

class MyGPS : public SFE_UBLOX_GNSS{
    public:
    MyGPS(byte address);

    Status status = Status::status_NACK;
    Mode mode = Mode::mode_RUN;
    IsWorking isWorking = IsWorking::isWorking_FALSE;

    bool setup(bool verbose = false);
    void getData();
    void printData();

    virtual bool setTimePulse();
    
    uint16_t year;
    uint8_t month;
    uint8_t day;

    uint8_t hour;
    uint8_t minute;
    uint8_t second;

    float latitude;
    float longitude;
    float altitude;
    byte siv;

    UBX_CFG_TP5_data_t timePulse;

    private:
    byte _address;
};

extern MyGPS gps;

#endif