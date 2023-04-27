#include "globalVars.h"

#include "myLora.h"

#include "RTOS_tasks.h"

#include "myTime.h"
#include "temperature.h"
#include "gps.h"
#include "myINA.h"
#include "mySD.h"
#include "myServer.h"
#include "myOxygen.h"
#include "myNeo.h"
#include "myINA.h"
#include "myCO2.h"
#include "myBNO.h"
#include "myBH1730.h"
#include "mySpectro.h"
#include "camera.h"

MyLora::MyLora(SPIClass* bus, double frequency, uint8_t cs, uint8_t reset, uint8_t dio0, uint8_t id, uint8_t txPower) : 
    LoRa_CanSat(bus, cs, reset, dio0),
    _txPower(txPower),
    _frequency(frequency),
    _cs(cs),
    _reset(reset),
    _dio0(dio0),
    _id(id){
}

bool MyLora::setup(bool verbose){
    verbose ? Serial.println("---LORA setup-------------------------------------") : 0;

    setPins(_cs, _reset, _dio0);
    if(!begin(_frequency)){
        isWorking = IsWorking::isWorking_FALSE;
        return false;
    }

    //dumpRegisters(Serial);
    
    enableCrc();
    setSyncWord(_id);
    setTxPower(_txPower);
    onReceive(onDio0Rise, onReceive_callback);
    onTxDone(onDio0Rise, onTxDone_callback);
    receive();

    isWorking = IsWorking::isWorking_TRUE;
    return true;
}

void MyLora::sendData(){

    switch(ackStatus){
        case 0:
            status = Status::status_FAIL;
            break;
        case 1:
            status = Status::status_NACK;
            break;
        case 2:
            status = Status::status_OK;
            break;
        default:
            status = Status::status_FAIL;
            break;
    }

    ackStatus = 0;

    if(isWorking == IsWorking::isWorking_FALSE){
        if(!setup()){
            status = Status::status_FAIL;
            return;
        }
    }

    if(!beginPacket()){
        isWorking = IsWorking::isWorking_FALSE;
        status = Status::status_FAIL;
        return;
    }

    /*  Header  */
    //write(_id);

    /*  Message  */
    myPrint(cycle);
    myPrint(sensorStatuses());
    myPrint(rtc.dateTime_short_string);
    myPrint(bh.lightIntensity, 1);
    myPrint(bme.temperature); myPrint(bme.pressure); myPrint(bme.humidity);
    myPrint(ina.voltage); myPrint(ina.current, 1); myPrint(ina.power, 0);
    myPrint(scd.co2);
    myPrint(gps.latitude); myPrint(gps.longitude); myPrint(gps.altitude); myPrint(gps.siv);
    myPrint(bno.roll, 0); myPrint(bno.pitch, 0); myPrint(bno.yaw, 0); // + posílat přetížení? 
    myPrint(oxygen.concentration, 1);
    for(int i = 0; i < 18; i++) myPrint(asx.data[i], 1);
    myPrint(refreshRate);

    packetLength = getPacketLength();

    myEndPacket(true);

    receive();
}

void MyLora::printStatus(){
    Serial.print("Sending data: ");
    if(status == Status::status_FAIL){
        Serial.println("FAIL");
        return;
    }

    if(status == Status::status_NACK){
        Serial.print("SEND");
    }
    else{
        //Serial.print("ACK - Incoming message: "); Serial.print(getFullMessage());
        Serial.print("ACK");
    }

    Serial.print(" - "); Serial.print(packetLength); Serial.println(" bytes");
    /*switch(lastAckStatus){
        case 1:
            Serial.println("SEND");
            break;
        case 2:
            Serial.println("ACK");
            break;
        default:
            Serial.println("NACK");
            break;
    }*/



}

ISR_PREFIX void IRAM_ATTR MyLora::onDio0Rise(){
    vTaskResume(isrHandleDioRise_handle);
}

void onReceive_callback(int packetSize){
    lora.ackStatus = 2;
    String message = "";
    for(int i = 0; i < packetSize; i++){
        char inChar = (char)lora.read();
        if((uint8_t)inChar <= 127 && (uint8_t)inChar != 0){
            message += inChar;
        }
    }
    if(!lora.checkCK(message)) return;
    while(lora.getDataLength() > 0){
        String cw = lora.getData();
        if(cw == "setMode"){
            Serial.println(lora.getData());
        }
        else if(cw == "camPower"){
            String command = lora.getData();

            if(command == "ON") cam.toggleOnOff(true);
            else if (command == "OFF") cam.toggleOnOff(false);
        }
        else if(cw == "camRec"){
            cam.toggleRec();
        }

        else if(cw == "restart"){
            ESP.restart();
        }

        else if(cw == "refreshRate"){
            refreshRate = lora.getData().toInt();
        }

        else if(cw == "updateStartingPressure"){
            bme.startingPressure = bme.pressure;
        }

        else if(cw == "neopixelMode"){
            String command = lora.getData();

            if(command == "OFF") neo.mode = 0;
            else if(command == "LOW") neo.mode = 2;
            else if(command == "FULL") neo.mode = 1;
        }

        else if(cw != "ACK"){
            Serial.print("Unknown: "); Serial.println(cw);
            return;
        }
    }
}

void onTxDone_callback(){
    if(lora.ackStatus != 2){
        lora.ackStatus = 1;
    }
}

uint8_t MyLora::singleTransfer(uint8_t address, uint8_t value){
  uint8_t response;

  xSemaphoreTake(spiSemaphore_hadle, portMAX_DELAY);

  digitalWrite(_cs, LOW);

  _spi->beginTransaction(_spiSettings);
  _spi->transfer(address);
  response = _spi->transfer(value);
  _spi->endTransaction();

  digitalWrite(_cs, HIGH);

  xSemaphoreGive(spiSemaphore_hadle);

  return response;
}