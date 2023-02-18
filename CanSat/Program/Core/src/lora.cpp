#include "globalVars.h"

MyLora::MyLora(SPIClass* bus, double frequency, uint8_t cs, uint8_t reset, uint8_t dio0, uint8_t id):
    _spiSettings(LORA_DEFAULT_SPI_FREQUENCY, MSBFIRST, SPI_MODE0),
    _spi(&LORA_DEFAULT_SPI){
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
    onTxDone(onTxDone_callback);
    receive();

    status = OK;
    return true;
}

void MyLora::dumpRegisters(Stream& out)
{
  for (int i = 0; i < 128; i++) {
    out.print("0x");
    out.print(i, HEX);
    out.print(": 0b");
    out.println(readRegister(i), BIN);
  }
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

int MyLora::endPacket(bool async)
{
  
  if ((async) && (_onTxDone))
      writeRegister(REG_DIO_MAPPING_1, 0x40); // DIO0 => TXDONE

  // put in TX mode
  writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

  if (!async) {
    // wait for TX done
    while ((readRegister(REG_IRQ_FLAGS) & IRQ_TX_DONE_MASK) == 0) {
      yield();
    }
    // clear IRQ's
    writeRegister(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK);
  }

  return 1;
}

void MyLora::onTxDone(void(*callback)()){
    _onTxDone = callback;
    if(callback){
        pinMode(_dio0, INPUT);
        attachInterrupt(digitalPinToInterrupt(_dio0), onDio0Rise, RISING);
        Serial.println("DIO0 interupt attached");
    }
    else{
        detachInterrupt(_dio0);
        Serial.println("DIO0 interupt detached");
    }
}

ISR_PREFIX void IRAM_ATTR MyLora::onDio0Rise(){
    //lora.handleDio0Rise();
    vTaskResume(isrHandleDioRise_handle);
}

void MyLora::handleDio0Rise(){
    Serial.println("Interupt?!");
    uint8_t irqFlags = readRegister(0x12);
    writeRegister(0x12, irqFlags);
}

uint8_t MyLora::readRegister(uint8_t address){
    return singleTransfer(address & 0x7f, 0x00);
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

uint8_t MyLora::singleTransfer(uint8_t address, uint8_t value)
{
  uint8_t response;

  digitalWrite(_cs, LOW);

  _spi->beginTransaction(_spiSettings);
  _spi->transfer(address);
  response = _spi->transfer(value);
  _spi->endTransaction();

  digitalWrite(_cs, HIGH);

  return response;
}