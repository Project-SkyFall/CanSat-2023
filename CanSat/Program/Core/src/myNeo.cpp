#include "globalVars.h"

#include "myNeo.h"

#include "myTime.h"
#include "temperature.h"
#include "gps.h"
#include "myLora.h"
#include "myINA.h"
#include "mySD.h"
#include "myServer.h"
#include "myOxygen.h"
#include "myINA.h"
#include "myCO2.h"
#include "myBNO.h"

/*MyNeo::MyNeo(uint16_t numPixels, uint8_t pin, neoPixelType type){
    _numPixels = numPixels;
    _pin = pin;
    _type = type;
}*/

MyNeo::MyNeo(uint16_t numPixels, uint8_t pin, neoPixelType type) :
  _numPixels(numPixels),
  _pin(pin),
  _type(type){}

bool MyNeo::setup(bool verbose){
    verbose ? Serial.println("---NeoPixel setup---------------------------------") : 0;
    updateType(_type);
    updateLength(_numPixels);
    setPin(_pin);
    
    begin();

    clear();

    return true;
}

//TODO make animated IC statuses

void MyNeo::animation(){
  /*static uint16_t ledStep;

  clear();
  if(ledStep >= 2*_numPixels - 1){
    ledStep = 0;
  }
  for(int l = 0; l<_numPixels; l++){
    int c = 255 - 255/_numPixels*(ledStep-l);
    if(c > 255 || c < 0){ c = 0;}
    setPixelColor(l, Color(0, c, c, 0));
  }
  show();
  ledStep++;
  vTaskDelay(40/portTICK_PERIOD_MS);*/


  //uint8_t *statusArray = new byte(_numPixels);
  //static uint8_t statusArray[_numPixels];
  static uint8_t step;
  static Status *statusArray = (Status *) malloc(_numPixels*sizeof(Status));

  if(step == 0){
    statusArray[0] = bme.status;
    statusArray[1] = lora.status;
    statusArray[2] = ds18.status;
    statusArray[3] = gps.status;
    statusArray[4] = sd.status;
    statusArray[5] = oxygen.status;
    statusArray[6] = rtc.status;
    statusArray[7] = bno.status;

    statusArray[8] = Status::status_SLEEP;
    statusArray[9] = Status::status_SLEEP;
    statusArray[10] = Status::status_SLEEP;
    statusArray[11] = Status::status_SLEEP;
    statusArray[12] = Status::status_SLEEP;
    statusArray[13] = Status::status_SLEEP;
    statusArray[14] = Status::status_SLEEP;
    statusArray[15] = Status::status_SLEEP;
  }

  clear();
  for(int i = 0; i < _numPixels/2; i++){
    setPixelColor(i, translateColor(statusArray[i], 10));
    setPixelColor(i+_numPixels/2, translateColor(statusArray[i+_numPixels/2], 10));
  }
  show();
  step++;
  if(step > 10){
    step = 0;
    //vTaskSuspend(NULL);
  }
  vTaskDelay(20/portTICK_PERIOD_MS);

}

uint32_t MyNeo::translateColor(Status status, byte brightness){

  if(brightness > 100) brightness = 100;

  if(status == Status::status_FAIL) return Adafruit_NeoPixel::Color(255*brightness/100, 0, 0, 0);

  if(status == Status::status_OK) return Adafruit_NeoPixel::Color(0, 255*brightness/100, 0, 0);

  if(status == Status::status_SLEEP) return Adafruit_NeoPixel::Color(0, 180*brightness/100, 255*brightness/100);

  return 0;

}