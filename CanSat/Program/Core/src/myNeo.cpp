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
#include "myBH1730.h"
#include "mySpectro.h"

/*MyNeo::MyNeo(uint16_t numPixels, uint8_t pin, neoPixelType type){
    _numPixels = numPixels;
    _pin = pin;
    _type = type;
}*/

MyNeo::MyNeo(uint16_t numPixels, uint8_t pin, uint8_t enablePin, neoPixelType type) :
  _numPixels(numPixels),
  _pin(pin),
  _type(type),
  _enablePin(enablePin){
    statusArray = (Status *) malloc(_numPixels*sizeof(Status));
  }

bool MyNeo::setup(bool verbose){
    verbose ? Serial.println("---NeoPixel setup---------------------------------") : 0;
    updateType(_type);
    updateLength(_numPixels);
    setPin(_pin);

    pinMode(_enablePin, OUTPUT);
    
    begin();

    clear();

    //mode == 2 ? digitalWrite(enablePin, HIGH) : digitalWrite(enablePin, LOW);

    return true;
}

//TODO make animated IC statuses

void MyNeo::updateStatuses(){
  statusArray[0] = bme.status;
  statusArray[1] = lora.status;
  statusArray[2] = ds18.status;
  statusArray[3] = gps.status;
  statusArray[4] = sd.status;
  statusArray[5] = oxygen.status;
  statusArray[6] = rtc.status;
  statusArray[7] = bno.status;

  statusArray[8] = scd.status;
  statusArray[9] = bh.status;
  statusArray[10] = asx.status;;
  statusArray[11] = Status::status_SLEEP;
  statusArray[12] = Status::status_SLEEP;
  statusArray[13] = Status::status_SLEEP;
  statusArray[14] = Status::status_SLEEP;
  statusArray[15] = Status::status_SLEEP;
}

void MyNeo::animation(){
  static uint8_t step;
  uint8_t brightness = 1;
  status = Status::status_SLEEP;

  if(mode == 0){
    status = Status::status_NACK;
    digitalWrite(enablePin, LOW);
    return;
  }

  digitalWrite(enablePin, HIGH);
  
  clear();

  if(mode == 1){
    brightness = 100;
    status = Status::status_OK;
  }

  for(int i = 0; i < _numPixels/2; i++){
    setPixelColor(i, translateColor(statusArray[i], brightness));
    setPixelColor(i+_numPixels/2, translateColor(statusArray[i+_numPixels/2], brightness));
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

  if(status == Status::status_FAIL) return Adafruit_NeoPixel::Color(255*brightness/100, 0, 0);

  if(status == Status::status_OK) return Adafruit_NeoPixel::Color(0, 255*brightness/100, 0);

  if(status == Status::status_SLEEP) return Adafruit_NeoPixel::Color(0, 180*brightness/100, 255*brightness/100);

  if(status == Status::status_NACK) return Adafruit_NeoPixel::Color(255*brightness/100, 175*brightness/100, 0*brightness/100);

  return 0;

}

void MyNeo::printStatus(){
  Serial.print("NEOPIXELS: ");
  if(status == Status::status_OK){
    Serial.println("FULL");
    return;
  }
  else if(status == Status::status_NACK){
    Serial.println("SUSPENDED");
    return;
  }
  Serial.println("RUNNING");
}