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

    //lastAckStatus = ackStatus;

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
    myPrint(rtc.dateTime_short_string);
    myPrint(bme.temperature); myPrint(bme.pressure); myPrint(bme.humidity);
    myPrint(ina.current); myPrint(ina.power);

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
        Serial.println("SEND");
        return;
    }

    Serial.println("ACK");


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

void MyLora::handleDio0Rise(){
    int irqFlags = readRegister(REG_IRQ_FLAGS);

    // clear IRQ's
    writeRegister(REG_IRQ_FLAGS, irqFlags);

    if ((irqFlags & IRQ_PAYLOAD_CRC_ERROR_MASK) == 0) {

        if ((irqFlags & IRQ_RX_DONE_MASK) != 0) {
            // received a packet
            _packetIndex = 0;

            // read packet length
            int packetLength = _implicitHeaderMode ? readRegister(REG_PAYLOAD_LENGTH) : readRegister(REG_RX_NB_BYTES);

            // set FIFO address to current RX address
            writeRegister(REG_FIFO_ADDR_PTR, readRegister(REG_FIFO_RX_CURRENT_ADDR));
           
            if (_onReceive) {
                _onReceive(packetLength);
            }
        }
        if ((irqFlags & IRQ_TX_DONE_MASK) != 0) {
            writeRegister(REG_DIO_MAPPING_1, 0x00); // DIO0 => RXDONE
            if (_onTxDone) {
                _onTxDone();
            }
        }
    }
}

void onReceive_callback(int packetSize){
    lora.ackStatus = 2;

    for(int i = 0; i < packetSize; i++){
        Serial.print((char)lora.read());
    }
    Serial.println();
}

void onTxDone_callback(){
    lora.ackStatus = 1;
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