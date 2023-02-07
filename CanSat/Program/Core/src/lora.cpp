#include "globalVars.h"

MyLora::MyLora(SPIClass* bus, double frequency, uint8_t cs, uint8_t reset, uint8_t dio0, uint8_t id){
    _bus = bus;
    _frequency = frequency;
    _cs = cs;
    _reset = reset;
    _dio0 = dio0;
    _id = id;
}

bool MyLora::setup(bool verbose){
    status = FAIL;
    verbose ? Serial.println("---LORA setup-------------------------------------") : 0;

    setPins(_cs, _reset, _dio0);
    if(!begin(_frequency)) return false; 
    
    setSyncWord(_id);
    //onReceivve(onReceive_callback);
    //onTxDone(onTxDone_callback);
    receive();

    status = OK;
    return true;
}

template <typename T> void MyLora::myPrint(T input){
    print(input); print(";");
}

void MyLora::sendData(){
    static int cycle;
    if(status == FAIL){
        if(!setup()) return;
    }

    if(!beginPacket()){
        status = FAIL;
        return;
    }
    /*  Header  */
    write(_id);

    /*  Message  */
    myPrint(rtc.dateTime_string);
    myPrint(bme.temperature); myPrint(bme.pressure); myPrint(bme.humidity);
    myPrint(ina.current); myPrint(ina.power);
    myPrint(cycle++);


    endPacket(true);

    receive();

    status = OK;
}

void MyLora::printStatus(){
    Serial.print("Sending data: ");
    status == OK ? Serial.println("OK") : Serial.println("FAIL");
}

void MyLora::onReceive(void(*callback)(int)){

    if(callback){
        pinMode(_dio0, INPUT);
        attachInterrupt(_dio0, onDio0Rise, RISING);
        Serial.println("DIO0 interupt attached");
    }
    else{
        detachInterrupt(_dio0);
        Serial.println("DIO0 interupt detached");
    }

}

void MyLora::onTxDone(void(*callback)()){
    _onTxDone = callback;
    if(callback){
        pinMode(_dio0, INPUT);
        attachInterrupt(_dio0, onDio0Rise, RISING);
        Serial.println("DIO0 interupt attached");
    }
    else{
        detachInterrupt(_dio0);
        Serial.println("DIO0 interupt detached");
    }
}

void MyLora::onDio0Rise(){
    Serial.println("Dio0Rised");
    //lora.handleDio0Rise();
}

void MyLora::handleDio0Rise(){
    Serial.println("Interupt?!");
    uint8_t irqFlags = readRegister(0x12, 0x00);
    Serial.println(irqFlags);
    writeRegister(0x12, irqFlags);
}

uint8_t MyLora::readRegister(uint8_t address, uint8_t value){
    return singleTransfer(address & 0x7f, value);
}

void MyLora::writeRegister(uint8_t address, uint8_t value){
    singleTransfer(address | 0x80, value);
}

uint8_t MyLora::singleTransfer(uint8_t address, uint8_t value){
    /*uint8_t response;

    digitalWrite(_cs, LOW);

    _spi->beginTransaction(_spiSettings);
    _spi->transfer(address);
    response = _spi->transfer(value);
    _spi->endTransaction();

    digitalWrite(_cs, HIGH);

    return response;*/
    return 0;
}

void onReceive_callback(int packetSize){
    if(packetSize == 0) return;
    // Header
    uint8_t id = lora.read();
    // Message
    String incoming = "";
    while(lora.available()){
        incoming += (char)lora.read();
    }
}

void onTxDone_callback(){
    Serial.println("Sending done");
}