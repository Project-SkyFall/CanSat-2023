#ifndef MY_NEO_H
#define MY_NEO_H

#include <Adafruit_NeoPixel.h>

class MyNeo : public Adafruit_NeoPixel{
    public:
    MyNeo(uint16_t numPixels, uint8_t pin, neoPixelType type);
    bool setup(bool verbose=false);
    void animation();

    byte status;

    private:
    uint16_t _numPixels;
    uint8_t _pin;
    neoPixelType _type;
};

extern MyNeo neo;

#endif