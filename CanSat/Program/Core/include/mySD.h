#ifndef MYSD_H
#define MYSD_H

#include <SD.h>
#include <FS.h>
//#include <SdFat.h>
#include "globalVars.h"

//template <class Vol, class Fmt>
#define USE_FCNTL_H 1
class MySD{

    uint8_t _cs;

    public:
    MySD(uint8_t cs);;
    bool setup(bool verbose = false);
    bool save();
    bool openFile();
    void printStatus();

    uint16_t postfix;
    //String postfix;
    String path;

    Status status = Status::status_NACK;
    Mode mode = Mode::mode_RUN;
    IsWorking isWorking = IsWorking::isWorking_FALSE;

    const uint8_t& cs = _cs;
};

template <typename T> void myPrint(T input);
template <typename T> void myPrintln(T input);

/*class MyFile{
    public:
    template <typename T> void myPrint(T input);
    template <typename T> void myPrintln(T input);
};*/

extern MySD sd;
extern File myFile;
extern bool fileOpened;

#endif