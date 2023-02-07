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
    void printStatus();

    uint16_t postfix;
    //String postfix;
    String path;
    byte status;

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

//extern SdFat SDfat;

#endif