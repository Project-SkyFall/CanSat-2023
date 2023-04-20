#include "mySpectro.h"

MySpectro::MySpectro(TwoWire &bus):
    _bus(bus){
}

bool MySpectro::setup(bool verbose){
    verbose ? Serial.println("---AS7265x setup----------------------------------") : 0;

    if(!begin(_bus)){
        status = Status::status_FAIL;
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }

    setMeasurementMode(3);
    setIntegrationCycles(49);

    return true;
}

void MySpectro::getData(){
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

    if(!isMeasuring){
        setMeasurementMode(3);
        isMeasuring = true;
        return;
    }

    if(!dataAvailable()) return;

    data[0] = getCalibratedA();
    data[1] = getCalibratedB();
    data[2] = getCalibratedC();
    data[3] = getCalibratedD();
    data[4] = getCalibratedE();
    data[5] = getCalibratedF();

    data[6] = getCalibratedG();
    data[7] = getCalibratedH();
    data[8] = getCalibratedR();
    data[9] = getCalibratedI();
    data[10] = getCalibratedS();
    data[11] = getCalibratedJ();

    data[12] = getCalibratedT();
    data[13] = getCalibratedU();
    data[14] = getCalibratedV();
    data[15] = getCalibratedW();
    data[16] = getCalibratedK();
    data[17] = getCalibratedL();

    for(int i = 0; i < 18; i++) data[i] = data[i]*6.83; //convert to Lux

    /*_bus.beginTransmission(_address);
    _bus.write(0x14);
    _bus.endTransmission();

    _bus.requestFrom(_address, )*/
    isMeasuring = false;
    status = Status::status_OK;
}

void MySpectro::printData(){
    Serial.print("AS7265X: ");
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
    for(int i = 0; i < 18; i++) Serial.print(data[i]); Serial.print(" lux - ");
    Serial.println();
}