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

    static bool initDone;

    if(verbose) Serial.println("---SDc setup-------------------------------------");

    if(!SD.begin(_cs)){
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }

    if(initDone) return true;

    //-- Initial setup only --------------------------------------------------

    while(true){
        postfix++;
        path = ("/datasaves/" + rtc.date_string + "/Project SkyFall-CanSat 2023 - " + rtc.date_string + " - " + postfix + ".csv");
        if(!SD.exists(path)){
            break;
        }
    }

    if(verbose) Serial.print("Creating data file: ");
    myFile = SD.open(path, FILE_APPEND, true);
    if(!myFile){
        verbose ? Serial.println("FAIL") : 0;
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }

    #ifndef VISU_BACKUP // Default datasaving
    myPrint("Sensor's status");
    myPrint("Date & Time");
    myPrint("Light intensity");
    myPrint("Temperature"); myPrint("Pressure"); myPrint("Humidity");
    myPrint("Voltage"); myPrint("Current"); myPrint("Power");
    myPrint("CO2");
    myPrint("Latitude"); myPrint("Longitude"); myPrint("Altitude"); myPrint("SIV");
    myPrint("Roll"); myPrint("Pitch"); myPrint("YaW"); myPrint("Accel X"); myPrint("Accel Y"); myPrint("Accel Z");
    for(int i = 1; i <= 18; i++) myPrint("ASX " + String(i));
    myPrintln("Oxygen");

    #else // Visualization backup data
    myPrint("batteryGS");
    myPrint("tempGS");
    myPrint("humidityGS");
    myPrint("pressGS");    
    myPrint("altGS");
    myPrint("timeGS");
    myPrint("latGS"); myPrint("longGS"); myPrint("rangeCan");
    myPrint("dORa4"); myPrint("dORa0"); myPrint("dORa3"); myPrint("dORa1"); myPrint("dORa2"); myPrint("dORa5"); myPrint("dORa7"); myPrint("dORa10"); myPrint("dORa8"); myPrint("dORa6"); myPrint("dORa9");
    myPrint("time");
    myPrint("lightIntensity");
    myPrint("temperature"); myPrint("pressure"); myPrint("humidity");
    myPrint("battery"); myPrint("current"); myPrint("power");
    myPrint("co2");
    myPrint("lat"); myPrint("long"); myPrint("height_GPS"); myPrint("height"); myPrint("sattelitesNum");
    myPrint("roll"); myPrint("pitch"); myPrint("yaw");
    myPrint("oxygen");
    myPrint("asx0"); myPrint("asx1"); myPrint("asx2");myPrint("asx3"); myPrint("asx4"); myPrint("asx5"); myPrint("asx6"); myPrint("asx7"); myPrint("asx8"); myPrint("asx9"); myPrint("asx10"); myPrint("asx11"); myPrint("asx12"); myPrint("asx13"); myPrint("asx14"); myPrint("asx15"); myPrint("asx16");myPrint("asx17");
    myPrint("transfer");
    myPrint("dORa11");
    myPrintln("time2");
    #endif

    myFile.flush();

    if(verbose) Serial.println(path);

    initDone = true;
    
    isWorking = IsWorking::isWorking_TRUE;
    return true;
}

bool MySD::save(){

    if(mode == Mode::mode_SLEEP){
        status = Status::status_SLEEP;
        return false;
    }

    xSemaphoreTake(spiSemaphore_hadle, portMAX_DELAY);
    if(!checkConnection()){ // Sendy dummy byte to check SD presense
        if(fileOpened){ // If file is still opened, close it and dismount logical drive
            myFile.close();
            SD.end();
            fileOpened = false;
        }
        if(!setup()){ // Try to remount logical drive
            status = Status::status_FAIL;
            xSemaphoreGive(spiSemaphore_hadle);
            return false;
        }
    }

    if(!fileOpened){ // Keep opening file until it is succesfull
        myFile = SD.open(path, FILE_APPEND);
        if(!myFile){
            status = Status::status_FAIL;
            return false;
        }
        fileOpened = true;
    }


    /*Saving data*/
    #ifndef VISU_BACKUP // Default datasaving
    myPrint(sensorStatuses());
    myPrint(rtc.dateTime_short_string);
    myPrint(bh.lightIntensity);
    myPrint(bme.temperature); myPrint(bme.pressure); myPrint(bme.humidity);
    myPrint(ina.voltage); myPrint(ina.current); myPrint(ina.power);
    myPrint(scd.co2);
    myPrint(gps.latitude); myPrint(gps.longitude); myPrint(gps.altitude); myPrint(gps.siv);
    myPrint(bno.roll); myPrint(bno.pitch); myPrint(bno.yaw); myPrint(bno.accel.x); myPrint(bno.accel.y); myPrint(bno.accel.z);
    for(int i = 0; i < 18; i++) myPrint(asx.data[i]);
    myPrintln(oxygen.concentration);

    #else // Visualization backup data
    myPrint(0);
    myPrint(0);
    myPrint(0);
    myPrint(0);    
    myPrint(0);
    myPrint(0);
    myPrint(0); myPrint(0); myPrint(0);
    myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0);
    myPrint(cycle);
    myPrint(bh.lightIntensity);
    myPrint(bme.temperature); myPrint(bme.pressure); myPrint(bme.humidity);
    myPrint(ina.voltage); myPrint(ina.current); myPrint(ina.power);
    myPrint(scd.co2);
    myPrint(gps.latitude); myPrint(gps.longitude); myPrint(gps.altitude); myPrint(gps.altitude); myPrint(gps.siv);
    myPrint(bno.roll); myPrint(bno.pitch); myPrint(bno.yaw);
    myPrint(oxygen.concentration);
    myPrint(0); myPrint(0); myPrint(0);myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0); myPrint(0);myPrint(0);
    myPrint(0);
    myPrint(0);
    myPrintln(cycle);
    #endif

    myFile.flush();

    xSemaphoreGive(spiSemaphore_hadle);
    
    status = Status::status_OK;
    return true;
}


void MySD::printStatus(){
    Serial.print("Saving data: ");
    if(status == Status::status_NACK){
        Serial.println("NACK");
        return;
    }
    status == Status::status_OK ? Serial.println("OK") : Serial.println("FAIL");
}

bool MySD::checkConnection(){

    digitalWrite(_cs, LOW);

    _spi->beginTransaction(_spiSettings);
    uint8_t response = _spi->transfer(0xFF);
    _spi->endTransaction();

    if(response == 0x00) return false;
    return true;
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