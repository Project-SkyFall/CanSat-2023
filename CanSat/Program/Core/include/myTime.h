#ifndef MYTIME_H
#define MYTIME_H

#include "globalVars.h"

class MyTime{
    public:
    bool setup(bool verbose = false);
    void printData();
    void getData();

    uint8_t day = 2;
    uint8_t month = 2;
    uint16_t year = 2023;

    uint8_t hour = 9;
    uint8_t minute = 0;
    uint8_t second = 0;

    String date_string;
    String dateTime_string;

    byte status;

};

void timeDateGetDataTask(void *pvParameters);

extern MyTime rtc;

#endif