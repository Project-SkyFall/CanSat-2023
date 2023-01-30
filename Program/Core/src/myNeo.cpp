#include "globalVars.h"

MyNeo::MyNeo(uint16_t numPixels, uint8_t pin, neoPixelType type){
    _numPixels = numPixels;
    _pin = pin;
    _type = type;
}

bool MyNeo::setup(bool verbose){
    verbose ? Serial.println("------------------NeoPixel setup------------------") : 0;
    updateType(_type);
    updateLength(_numPixels);
    setPin(_pin);
    
    neo.begin();

    return true;
}

void MyNeo::animation(){
  static unsigned long prevMillis;
  static uint16_t ledStep;

  if(millis() - prevMillis > 40){
    clear();
    prevMillis = millis();
    if(ledStep >= 2*_numPixels - 1){
      ledStep = 0;
    }
    for(int l = 0; l<_numPixels; l++){
      int c = 255 - 255/_numPixels*(ledStep-l);
      if(c > 255 || c < 0){ c = 0;}
      setPixelColor(l, Color(0, c, 0, 0));
    }
    show();
    ledStep++;
  }
}