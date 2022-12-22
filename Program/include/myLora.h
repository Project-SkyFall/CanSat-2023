#ifndef MYLORA_H
#define MYLORA_H

#include <LoRa.h>

class MyLora : public LoRaClass{
    public:
    bool setup();
    void send();
};

extern MyLora lora;

#endif