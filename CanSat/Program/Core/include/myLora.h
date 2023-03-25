#ifndef MYLORA_H
#define MYLORA_H

#include <LoRa_CanSat.h>

#include "globalVars.h"

#if (ESP8266 || ESP32)
    #define ISR_PREFIX ICACHE_RAM_ATTR
#else
    #define ISR_PREFIX
#endif

class MyLora : public LoRa_CanSat{
    public: 
    MyLora(SPIClass* bus, double frequency, uint8_t cs, uint8_t reset, uint8_t dio0, uint8_t id, uint8_t txPower = 17);
    bool setup(bool verbose=false);
    void sendData();
    void printStatus();

    int endPacket(bool async=false);

    void onReceive(void(*callback)(int));
    void onTxDone(void(*callback)());
    static void onDio0Rise();
    void handleDio0Rise();

    bool checkTxDone();
    
    //template <typename T> void myPrint(T input);
    void dumpRegisters(Stream& out);

    Status status = Status::status_NACK;
    Mode mode = Mode::mode_RUN;
    IsWorking isWorking = IsWorking::isWorking_FALSE;


    bool isTxDone;
    bool stopCheckTxDone;

    const uint8_t& cs = _cs;

    private:
    SPISettings _spiSettings;
    SPIClass* _spi;
    double _frequency;
    uint8_t _cs;
    uint8_t _reset;
    uint8_t _dio0;
    uint8_t _id;
    uint8_t _txPower;
    
    void (*_onReceive)(int);
    void (*_onTxDone)();

    uint8_t readRegister(uint8_t address);
    void writeRegister(uint8_t address, uint8_t value);
    uint8_t singleTransfer(uint8_t address, uint8_t value);
};

void onReceive_callback(int packetSize);
void onTxDone_callback();

extern MyLora lora;

#endif