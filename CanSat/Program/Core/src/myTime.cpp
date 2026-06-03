#include "globalVars.h"

#include "myTime.h"

MyTime::MyTime(uint8_t address, TwoWire *bus):
    _address(address),
    _bus(bus){
    }

bool MyTime::setup(bool verbose){
    verbose ? Serial.println("---RTC setup--------------------------------------") : 0;

    if(!begin()){
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }
    /*setTime(23, 6, 0);
    setDate(28, 3, 2023);*/
    getData();

    status = Status::status_OK;
    return true;
}

void MyTime::getData(){
    getTime(hour, minute, second);
    getDate(day, month, year);

    date_string = ((String)day + "." + (String)month + "." + (String)year);
    dateTime_string = ((String)day + "." + (String)month + "." + (String)year + "-" + (String)hour + ":" + (String)minute + ":" + (String)second);
    dateTime_short_string = timeDateToString(day, month, year, hour, minute, second);

    status = Status::status_OK;
}

void MyTime::printData(){
    Serial.print("RTC: ");
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

    Serial.println(dateTime_string);
}

bool MyTime::begin(){
    _bus->beginTransmission(_address);
    if(_bus->endTransmission() != 0) return false;

    readRegister(0x02, 1);
    writeRegister(0x02, _bus->read() & 0b10111111); // Sets RTC to EU time format (US time format currently not supported)
    
    readRegister(0xF, 1);
    uint8_t value = (_bus->read() & 128);
    if(value == 128){
        _dataCorupted = true;
        writeRegister(0x0F, 0); // NULLs status register
        _verbosePrint ? Serial.println("Data corrupted, OSC stopped! Bit cleared") : 0;
    }
    
    return true;
}

void MyTime::printVerbose(bool input){
    _verbosePrint = input;
}

bool MyTime::getTime(uint8_t &hour, uint8_t &minute, uint8_t &second){

    if(readRegister(0x00, 3) == 0) return false;
    uint8_t sec_register = _bus->read();
    uint8_t min_register = _bus->read();
    uint8_t hour_register = _bus->read();

    second = resolveByte(sec_register, 6);
    minute = resolveByte(min_register, 6);
    hour = resolveByte(hour_register, 5);

    return true;
}

bool MyTime::getDate(uint8_t &day, uint8_t &month, uint16_t &year){

    if(readRegister(0x04, 3) == 0) return false;
    uint8_t day_register = _bus->read();
    uint8_t month_register = _bus->read();
    uint8_t year_register = _bus->read();

    day = resolveByte(day_register, 5);
    
    month = resolveByte(month_register, 4);

    year = resolveByte(year_register, 7);
    year += (month_register >> 7)*100;
    year += 2000;

    return true;
}

String MyTime::timeDateToString(uint8_t day, uint8_t month, uint16_t year, uint8_t hour, uint8_t minute, uint8_t second){
    year = year - 2000;
    if(year >= 100) year - 100;

    uint8_t array[6] = {day, month, (uint8_t)year, hour, minute, second};
    String output;
    for(int i = 0; i < 6; i++){
        if(array[i] < 10) output += "0";
        output += String(array[i]);
    }
    return output;
}

uint8_t MyTime::setTime(uint8_t hour, uint8_t minute, uint8_t second){
    //writeRegister(convertToBCD(hour));
    _bus->beginTransmission(_address);
    //_bus->write(hour/10 << 4)
    _bus->write(0x00);
    _bus->write(convertToBCD(second, 6));
    _bus->write(convertToBCD(minute, 6));
    _bus->write(convertToBCD(hour, 5));
    return _bus->endTransmission();
}

uint8_t MyTime::setDate(uint8_t day, uint8_t month, uint16_t year){
    uint8_t century = 0;
    _bus->beginTransmission(_address);
    _bus->write(0x04);

    year -= 2000;
    if(year >= 100){
        year -= 100;
        century = 128;
    }

    _bus->write(convertToBCD(day, 5));
    _bus->write(convertToBCD(month, 4) | century);
    _bus->write(convertToBCD(year, 7));

    return _bus->endTransmission();
}

uint8_t MyTime::convertToBCD(uint8_t input, uint8_t lastBit){
    input = input & (uint8_t)((pow(2, lastBit+1))-1);
    uint8_t output = ((input / 10) << 4) | (input % 10);
    return output;
}

template <uint8_t N> uint8_t MyTime::writeRegister(uint8_t regNum, uint8_t (&data)[N]){
    _bus->beginTransmission(_address);
    _bus->write(regNum);

    for(uint8_t i; i < N; i++){
        _bus->write(data[i]);
    }

    return _bus->endTransmission();
}

uint8_t MyTime::writeRegister(uint8_t regNum, uint8_t data){
    _bus->beginTransmission(_address);
    _bus->write(regNum);

    _bus->write(data);

    return _bus->endTransmission();
}

uint8_t MyTime::readRegister(uint8_t regNum, uint8_t size){
    _bus->beginTransmission(_address);
    _bus->write(regNum);
    _bus->endTransmission();

    return _bus->requestFrom(_address, size);
}

uint16_t MyTime::resolveByte(uint8_t input, uint8_t lastBit){
    /*uint16_t output;
    uint8_t multiplier = 10;
    uint8_t endBits = (input & 0b00001111);
    for(uint8_t i=4; i <= lastBit; i++){
        output += (((input >> i) & 0b00000001) * multiplier);
        multiplier = multiplier*2;
    }
    return output+endBits;*/
    input = input & (uint8_t)((pow(2, lastBit+1))-1);
    return ((input >> 4)*10 + (input & 15));
}