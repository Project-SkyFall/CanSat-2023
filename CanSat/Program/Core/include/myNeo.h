#ifndef MY_NEO_H
#define MY_NEO_H

#include <Adafruit_NeoPixel.h>

class MyNeo : public Adafruit_NeoPixel{
    public:
    MyNeo(uint16_t numPixels, uint8_t pin, uint8_t enablePin, neoPixelType type);
    bool setup(bool verbose=false);
    void animation();
    void printStatus();
    uint32_t translateColor(Status status, byte brightness);
    void updateStatuses();

    Status status;
    Status *statusArray;
    const uint8_t &enablePin = _enablePin;

    private:
    uint16_t _numPixels;
    uint8_t _pin;
    uint8_t _enablePin;
    neoPixelType _type;
};

extern MyNeo neo;

#endif