#include "globalVars.h"

#include "mySD.h"

#include "myTime.h"
#include "temperature.h"
#include "gps.h"
#include "myLora.h"
#include "myINA.h"
#include "mySD.h"
#include "myServer.h"
#include "myOxygen.h"
#include "myNeo.h"
#include "myINA.h"
#include "myCO2.h"
#include "myBNO.h"

MySD::MySD(uint8_t cs){
    _cs = cs;
}

bool MySD::setup(bool verbose){
    verbose ? Serial.println("---SDc setup-------------------------------------") : 0;

    static bool firstTime = true;

    if(!firstTime){
        SD.end();
    }

    while(!SD.begin(_cs)){
        byte i;
        i++;
        if(i > 5){
            status = Status::status_FAIL;
            return false;
        }
    }

    if(!firstTime){
        status = Status::status_OK;
        return true;
    }

    while(true){
        firstTime = false;
        postfix++;
        path = ("/datasaves/" + rtc.date_string + "/Project SkyFall-CanSat 2023 - " + rtc.date_string + " - " + postfix + ".csv");
        if(!SD.exists(path)){
            break;
        }
    }
    Serial.print("Creating data file: ");
    myFile = SD.open(path, FILE_WRITE, true);
    if(!myFile){
        Serial.println("FAIL");
        return false;
    }
    myPrint("Timestamp");
    myPrint("Temperature"); myPrint("Pressure"); myPrintln("Humidity");

    myFile.close();
    Serial.println(path);
    
    status = Status::status_OK;
    return true;
}

bool MySD::save(){
    if(status == Status::status_FAIL && !setup()){
        //Serial.println("SD card disconnected");
        return false;
    }
    //Serial.print("Saving data: ");
    myFile = SD.open(path, FILE_WRITE);
    if(!myFile){
        status = Status::status_FAIL;
        //Serial.println("FAIL");
        return false;
    }

    myPrint(rtc.dateTime_string);
    myPrint(bme.temperature); myPrint(bme.pressure); myPrintln(bme.humidity);

    myFile.close();
    //Serial.println("OK");

    status = Status::status_OK;
    return true;
}

void MySD::printStatus(){
    Serial.print("Saving data: ");
    status == Status::status_OK ? Serial.println("OK") : Serial.println("FAIL");
}

template <typename T> void /*MyFile::*/myPrint(T input){
    myFile.print(input); myFile.print(';');
}

template <typename T> void /*MyFile::*/myPrintln(T input){
    myFile.print(input); myFile.println(';');
}

/*void dateTime(uint16_t* date, uint16_t* time) {
 DateTime now = RTC.now();
 sprintf(timestamp, "%02d:%02d:%02d %2d/%2d/%2d \n", now.hour(),now.minute(),now.second(),now.month(),now.day(),now.year()-2000);
 Serial.println("yy");
 Serial.println(timestamp);
 // return date using FAT_DATE macro to format fields
 *date = FAT_DATE(now.year(), now.month(), now.day());

 // return time using FAT_TIME macro to format fields
 *time = FAT_TIME(now.hour(), now.minute(), now.second());
}*/