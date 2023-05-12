#include "myBH1730.h"

MyBH1730::MyBH1730(BH1730_GAIN gain, TwoWire* bus):
    _bus(bus),
    _gain(gain){
}

bool MyBH1730::setup(bool verbose){
    verbose ? Serial.println("---BH1730 setup-----------------------------------") : 0;

    if(!begin()){
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }

    setGain(BH1730_GAIN::GAIN_X1);
    setIntegrationCycle(245);
    
    return true;
}

void MyBH1730::getData(){
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

    lightIntensity = readLux();

    status = Status::status_OK;
}

void MyBH1730::printData(){
    Serial.print("BH1730: ");
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
    Serial.print(lightIntensity); Serial.println(" lux");
}

bool MyBH1730::begin(){
    openChannel(0x12);
    if(_bus->endTransmission() != 0) return false;

    _bus->requestFrom(_address, 1);
    uint8_t receive = _bus->read() >>4;
    if(receive != 0b0111) return false;

    setGain(_gain);

    return true;
    
}

void MyBH1730::setGain(BH1730_GAIN gain){
    _gain = gain;

    openChannel(0x07);
    _bus->write(static_cast<int>(_gain));
    _bus->endTransmission();
}

bool MyBH1730::setIntegrationCycle(uint8_t value){
    if(value == 0) return false;
    _integrationCycle = value;

    openChannel(0x01);
    _bus->write(value);
    if(_bus->endTransmission() == 0) return true;
    return false;
}

float MyBH1730::readLux(){
    int ITIME_ms = 2.8/1000 * 964 * (256-_integrationCycle);
    int Tmt = (ITIME_ms + 2.8/1000 * 714) + 5; //2.8/1000 * 964 * (256-218) + 2.8/1000 * 714

    /*specialCommand(0b00011);
    _bus->endTransmission();*/

    openChannel(0x00);
    _bus->endTransmission();

    _bus->requestFrom(_address, 1);
    uint8_t ret = _bus->read();

    openChannel(0x00);
    _bus->write(ret | 0b00000011);
    _bus->endTransmission();

    delay(Tmt);

    openChannel(0x14);
    _bus->endTransmission();

    _bus->requestFrom(_address, 4);

    uint16_t data0 = (_bus->read() | (_bus->read() <<8));
    uint16_t data1 = (_bus->read() | (_bus->read() <<8));
    if(data0 == 0) return 0;


    uint8_t gain = 0;
    if(_gain == BH1730_GAIN::GAIN_X1) gain = 1;
    else if(_gain == BH1730_GAIN::GAIN_X2) gain = 2;
    else if(_gain == BH1730_GAIN::GAIN_X64) gain = 64;
    else if(_gain == BH1730_GAIN::GAIN_X128) gain = 128;
    else gain = 1;

    if (data1/data0<0.26) return (( 1.290 * data0 - 2.733 * data1 ) / gain * 102.6 / ITIME_ms);
    if (data1/data0<0.55) return (( 0.795 * data0 - 0.859 * data1 ) / gain * 102.6 / ITIME_ms);
    if (data1/data0<1.09) return (( 0.510 * data0 - 0.345 * data1 ) / gain * 102.6 / ITIME_ms);
    if (data1/data0<2.13) return (( 0.276 * data0 - 0.130 * data1 ) / gain * 102.6 / ITIME_ms);
    return 0;
}

void MyBH1730::openChannel(uint8_t address){
    _bus->beginTransmission(_address);
    _bus->write(address | 0b10000000);
}

void MyBH1730::specialCommand(uint8_t command){
    _bus->beginTransmission(_address);
    _bus->write(command | 0b11100000);
}