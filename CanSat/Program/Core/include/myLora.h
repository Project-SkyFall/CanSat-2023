#ifndef MYLORA_H
#define MYLORA_H

#include <LoRa.h>

#define loraCSpin 32

class MyLora : public LoRaClass{
    public: 
    MyLora(SPIClass* bus, double frequency, uint8_t cs, uint8_t reset, uint8_t dio0, uint8_t id);
    bool setup(bool verbose=false);
    void sendData();
    void printStatus();

    void onReceive(void(*callback)(int));
    void onTxDone(void(*callback)());
    static void onDio0Rise();
    void handleDio0Rise();
    
    template <typename T> void myPrint(T input);

    byte status;

    const uint8_t& cs = _cs;

    private:
    SPIClass* _bus;
    double _frequency;
    uint8_t _cs;
    uint8_t _reset;
    uint8_t _dio0;
    uint8_t _id;
    
    void (*_onReceive)(int);
    void (*_onTxDone)();

    uint8_t readRegister(uint8_t address, uint8_t value);
    void writeRegister(uint8_t address, uint8_t value);
    uint8_t singleTransfer(uint8_t address, uint8_t value);
};

void onReceive_callback(int packetSize);
void onTxDone_callback();

extern MyLora lora;

#endif