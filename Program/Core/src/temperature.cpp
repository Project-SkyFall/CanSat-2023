#include "globalVars.h"
#include "temperature.h"

bool MyBme::setup(bool debug){
    address = 0x76;
    debug ? Serial.println("---BME setup---") : 0;
    status = FAIL;
    if(!begin(address)) return false;

    setSampling(Adafruit_BME280::MODE_FORCED,
                Adafruit_BME280::SAMPLING_X1,
                Adafruit_BME280::SAMPLING_X1,
                Adafruit_BME280::SAMPLING_X1,
                Adafruit_BME280::FILTER_OFF);

    status = OK;
    return true;
}

bool MyDS18B20::setup(bool verbose){
    verbose ? Serial.println("-----------------DS18B20 setup------------------") : 0;
    begin();

    /*if(!isConnected(&4)){
        status = FAIL;
        return false;
    }*/
    status = OK;
    return true;
}

void MyBme::getData(){
    if(status == SLEEP) return;

    if(!wireCheck(address)){
        status = FAIL;
        return;
    }
    else if(status == FAIL){
        if(!setup()) return;
    }

    takeForcedMeasurement();
    temperature = readTemperature();
    pressure = readPressure() / 100.0F;
    humidity = readHumidity();

    status = OK;
}

void MyDS18B20::getData(){
    if(status == SLEEP) return;

    /*if(!isConnected(&_pin)){
        if(!setup()){
            status = FAIL;
            return;
        }
    }*/
    requestTemperatures();

    temperature = getTempCByIndex(0);

}

void MyBme::printData(){
    Serial.print("BME280: ");
    if(status == SLEEP){
        Serial.println("SLEEPING");
        return;
    }
    else if(status == FAIL){
        Serial.println("FAILED");
        return;
    }
    Serial.print(temperature); Serial.print(" °C - ");
    Serial.print(pressure); Serial.print(" hPa - ");
    Serial.print(humidity); Serial.println(" %");
}

void MyDS18B20::printData(){
    Serial.print("DS18B20: ");
    if(status == SLEEP){
        Serial.println("SLEEPING");
        return;
    }
    else if(status == FAIL){
        Serial.println("FAILED");
        return;
    }
    Serial.print(temperature); Serial.println(" °C");
}
