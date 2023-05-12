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
#include "RTOS_tasks.h"
#include "mySpectro.h"
#include "myBH1730.h"

bool fileOpened;

MySD::MySD(uint8_t cs):
    _cs(cs){
}

bool MySD::setup(bool verbose){
    if(mode == Mode::mode_SLEEP){
        status = Status::status_SLEEP;
        return false;
    }
    static bool firstTime = true;
    verbose ? Serial.println("---SDc setup-------------------------------------") : 0;

    if(!SD.begin(_cs)){
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }

    if(!firstTime){
        isWorking = IsWorking::isWorking_TRUE;
        return true;
    }

    firstTime = false;
    
    while(true){
        postfix++;
        path = ("/datasaves/" + rtc.date_string + "/Project SkyFall-CanSat 2023 - " + rtc.date_string + " - " + postfix + ".csv");
        if(!SD.exists(path)){
            break;
        }
    }
    verbose ? Serial.print("Creating data file: "): 0;
    myFile = SD.open(path, FILE_APPEND, true);
    if(!myFile){
        verbose ? Serial.println("FAIL") : 0;
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }
    myPrint("Timestamp");
    myPrint("Temperature"); myPrint("Pressure"); myPrintln("Humidity");

    myFile.close();

    
    verbose ? Serial.println(path) : 0;
    
    isWorking = IsWorking::isWorking_TRUE;
    return true;
}

bool MySD::openFile(){

    static uint8_t tryNumber;

    if(mode == Mode::mode_SLEEP){
        status = Status::status_SLEEP;
        return false;
    }

    //if(isWorking == IsWorking::isWorking_FALSE){
        if(!setup()){
            tryNumber = 1;
            status = Status::status_FAIL;
            return false;
        }
    //}

    myFile = SD.open(path, FILE_APPEND);
    if(!myFile){
        SD.end();
        isWorking = IsWorking::isWorking_FALSE;
        status = Status::status_FAIL;
        return false;
    }
    return true;
}

bool MySD::save(){

    if(mode == Mode::mode_SLEEP){
        status = Status::status_SLEEP;
        return false;
    }

    if(isWorking == IsWorking::isWorking_FALSE){
        status = Status::status_FAIL;
        return false;
    }

    /*Saving data*/
    if(fileOpened){
        xSemaphoreTake(spiSemaphore_hadle, portMAX_DELAY);

        myPrint(cycle);
        myPrint(sensorStatuses());
        myPrint(rtc.dateTime_short_string);
        myPrint(bh.lightIntensity);
        myPrint(bme.temperature); myPrint(bme.pressure); myPrint(bme.humidity);
        myPrint(ina.voltage); myPrint(ina.current); myPrint(ina.power);
        myPrint(scd.co2);
        myPrint(gps.latitude); myPrint(gps.longitude); myPrint(gps.altitude); myPrint(gps.siv);
        myPrint(bno.roll); myPrint(bno.pitch); myPrint(bno.yaw); // + posílat přetížení? 
        myPrint(oxygen.concentration);
        for(int i = 0; i < 18; i++) myPrint(asx.data[i]);
        myPrintln(refreshRate);

        myFile.close();
        SD.end();
        fileOpened = false;

        xSemaphoreGive(spiSemaphore_hadle);
        xSemaphoreGive(saveData_semaphore);
        
        status = Status::status_OK;
        return true;
    }

    return false;

}


void MySD::printStatus(){
    Serial.print("Saving data: ");
    if(status == Status::status_NACK){
        Serial.println("NACK");
        return;
    }
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