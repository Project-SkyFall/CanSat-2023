#ifndef MYLORA_H
#define MYLORA_H

#include <LoRa.h>

class MyLora : public LoRaClass{
    public:
    byte status;
    bool setup(bool verbose);
    void send();
    void printStatus();
};

extern MyLora lora;

#endif