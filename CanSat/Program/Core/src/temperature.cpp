#include "globalVars.h"

#include "temperature.h"

MyBme::MyBme(byte address){
    _address = address;
}

bool MyBme::setup(bool debug){
    debug ? Serial.println("---BME setup--------------------------------------") : 0;

    if(!begin(_address)){
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }

    setSampling(Adafruit_BME280::MODE_FORCED,
                Adafruit_BME280::SAMPLING_X1,
                Adafruit_BME280::SAMPLING_X1,
                Adafruit_BME280::SAMPLING_X1,
                Adafruit_BME280::FILTER_OFF);

    isWorking = IsWorking::isWorking_TRUE;
    return true;
}

MyDS18B20::MyDS18B20(OneWire* bus, uint8_t pin):
    _bus(bus),
    _pin(pin)
    {}

bool MyDS18B20::setup(bool verbose){
    verbose ? Serial.println("---DS18B20 setup----------------------------------") : 0;
    setOneWire(_bus);
    begin();

    /*if(!isConnected(&4)){
        status = FAIL;
        return false;
    }*/
    isWorking = IsWorking::isWorking_TRUE;
    return true;
}

void MyBme::getData(){
    status = Status::status_NACK;

    if(mode == Mode::mode_SLEEP){
        status = Status::status_SLEEP;
        return;
    }

    if(!wireCheck(_address)){
        isWorking = IsWorking::isWorking_FALSE;
        status = Status::status_FAIL;
        return;
    }

    if(isWorking == IsWorking::isWorking_FALSE){
        if(!setup()){
        status = Status::status_FAIL;
        return;
        }
    }

    takeForcedMeasurement();
    temperature = readTemperature();
    pressure = readPressure() / 100.0F;
    humidity = readHumidity();

    static bool oneTime = false;
    if(!softwareReset && !oneTime){
        startingPressure = pressure;
        oneTime = true;
    }

    status = Status::status_OK;
}

void MyDS18B20::getData(){
    if(mode == Mode::mode_SLEEP){
        status = Status::status_SLEEP;
        return;
    }

    if(isWorking == IsWorking::isWorking_FALSE){
        if(!setup()){
            status = Status::status_FAIL;
            return;
        }
    }
    /*status = Status::status_FAIL;
    return;*/
    requestTemperatures();
    temperature = getTempCByIndex(0);

    status = Status::status_OK;
}

void MyBme::printData(){
    Serial.print("BME280: ");
    if(status == Status::status_SLEEP){
        Serial.println("SLEEPING");
        return;
    }
    else if(status == Status::status_FAIL){
        Serial.println("FAILED");
        return;
    }
    else if(status == Status::status_NACK){
        Serial.println("NACK");
        return;
    }
    
    Serial.print(temperature); Serial.print(" °C - ");
    Serial.print(pressure); Serial.print(" hPa - ");
    Serial.print(humidity); Serial.println(" %");
}

void MyDS18B20::printData(){
    Serial.print("DS18B20: ");
    if(status == Status::status_SLEEP){
        Serial.println("SLEEPING");
        return;
    }
    else if(status == Status::status_FAIL){
        Serial.println("FAILED");
        return;
    }
    else if(status == Status::status_NACK){
        Serial.println("NACK");
        return;
    }

    Serial.print(temperature); Serial.println(" °C");
}
