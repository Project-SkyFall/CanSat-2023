#ifndef MYTIME_H
#define MYTIME_H

#include "globalVars.h"

class MyTime{
    public:
    MyTime(uint8_t address, TwoWire *bus = &Wire);
    bool setup(bool verbose = false);
    void printData();
    void getData();

    bool begin();
    bool getTime(uint8_t &hour, uint8_t &minute, uint8_t &second);
    bool getDate(uint8_t &day, uint8_t &month, uint16_t &year);
    uint16_t resolveByte(uint8_t input, uint8_t lastBit);
    void printVerbose(bool input);
    uint8_t convertToBCD(uint8_t input, uint8_t lastBit = 7);
    uint8_t setTime(uint8_t hour, uint8_t minute, uint8_t second);
    uint8_t setDate(uint8_t day, uint8_t month, uint16_t year);
    String timeDateToString(uint8_t day, uint8_t month, uint16_t year, uint8_t hour, uint8_t minute, uint8_t second);

    uint8_t day = 31;
    uint8_t month = 12;
    uint16_t year = 2022;

    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t second = 0;

    String date_string;
    String dateTime_string;
    String dateTime_short_string;

    Status status;
    IsWorking isWorking;
    Mode mode;

    private:
    uint16_t _address;
    TwoWire *_bus;
    template <uint8_t N> uint8_t writeRegister(uint8_t regNum, uint8_t (&data)[N]);
    uint8_t writeRegister(uint8_t regNum, uint8_t data);
    uint8_t readRegister(uint8_t regNum, uint8_t size);
    bool _dataCorupted;
    bool _verbosePrint;
    bool _timeSetting;
};

extern MyTime rtc;

#endif