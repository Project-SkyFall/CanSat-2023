#include "globalVars.h"

#define sdCSpin 26

bool MySD::setup(bool verbose){
    verbose ? Serial.println("---SDc setup---") : 0;

    static bool firstTime = true;

    if(!firstTime){
        SD.end();
    }

    if(!SD.begin(sdCSpin)){
        status = FAIL;
        return false;
    }

    
    if(!firstTime){
        status = OK;
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
    
    status = OK;
    return true;
}

bool MySD::save(){
    if(status == FAIL && !setup()){
        //Serial.println("SD card disconnected");
        return false;
    }
    //Serial.print("Saving data: ");
    myFile = SD.open(path, FILE_WRITE);
    if(!myFile){
        status = FAIL;
        //Serial.println("FAIL");
        return false;
    }

    myPrint(rtc.dateTime_string);
    myPrint(bme.temperature); myPrint(bme.pressure); myPrintln(bme.humidity);

    myFile.close();
    //Serial.println("OK");

    status = OK;
    return true;
}

void MySD::printStatus(){
    Serial.print("Saving data: ");
    status == OK ? Serial.println("OK") : Serial.println("FAIL");
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