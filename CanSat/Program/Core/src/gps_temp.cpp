/*#include "globalVars.h"
#include "gps_temp.h"

unsigned long prevMillisGPSlostFix;

//byte isGPSok;
uint32_t prevCharsProcessed;
byte gpsFailedCycles;
double longitude, latitude, gpsAltitude, gpsSpeed, gpsCourse, gpsCourseTo, gpsDistanceBetween;
uint32_t gpsTime, satellitesNumber;
double targetLat, targetLng;
bool prevGpsStatus;

byte gpsStatus;
  #define GPS_LOCATION_UPDATED 7
  #define GPS_ALTITUDE_UPDATED 6
  #define GPS_SPEED_UPDATED 5
  #define GPS_TIME_UPDATED 4
  #define GPS_SATELLITES_UPDATED 3

  
  #define GPS_STATUS 0

byte gpsSetup(){
  Serial.println("---------------GPS setup----------------");
  prevMillisGPSlostFix = millis();
  while(millis() - prevMillisGPSlostFix < 2000 && gps.charsProcessed() <= 25){
    gps.encode(Serial1.read());
  }
  Serial.print("Recieved ");
  Serial.print(gps.charsProcessed());
  Serial.println(" chars");
  if(gps.charsProcessed() >= 20){
    bitWrite(gpsStatus, GPS_STATUS, OK);
    return OK;
  }
  bitWrite(gpsStatus, GPS_STATUS, FAIL);
  return FAIL;
}

void gpsFeedObject(){
  while(Serial1.available() > 0){
    gps.encode(Serial1.read());
    //bitWrite(gpsStatus, DEVICE_READY, 1);
    prevMillisGPSlostFix = millis();
    //satelitesNumber = 0;
  }
}

void gpsGetData(){
  prevGpsStatus = bitRead(gpsStatus, GPS_STATUS);
  gpsStatus = 0;
  bitWrite(gpsStatus, GPS_STATUS, prevGpsStatus);
  if(gps.charsProcessed() - prevCharsProcessed < 1){
    gpsFailedCycles++;
    if(gpsFailedCycles >= 2){
      /*prevMillisGPSlostFix = millis();
      bitWrite(gpsStatus, DEVICE_ERROR, 1);
      bitWrite(gpsStatus, GPS_LOST_FIX, 1);*/
      //Serial.print("No chars");
      /*bitWrite(gpsStatus, GPS_STATUS, FAIL);
    }
  }
  else{
    gpsFailedCycles = 0;
    bitWrite(gpsStatus, GPS_STATUS, OK);
    //Serial.print("Set to one");
  }
  prevCharsProcessed = gps.charsProcessed();
  
  if(gps.location.isUpdated() && gps.location.isValid()){
    bitWrite(gpsStatus, GPS_LOCATION_UPDATED, OK);
    latitude = gps.location.lat();
    longitude = gps.location.lng(); 
    //gpsCourseTo = gps.courseTo(latitude, longitude, targetLat, targetLng); 
    //gpsDistanceBetween = gps.distanceBetween(latitude, longitude, targetLat, targetLng);  
  }
  if(gps.altitude.isUpdated() && gps.altitude.isValid()){
    bitWrite(gpsStatus, GPS_ALTITUDE_UPDATED, OK);
    gpsAltitude = gps.altitude.meters();
  }
  if(gps.speed.isUpdated() && gps.speed.isValid()){
    bitWrite(gpsStatus, GPS_SPEED_UPDATED, OK);
    gpsSpeed = gps.speed.kmph();
  }
  if(gps.time.isUpdated() && gps.time.isValid()){
    bitWrite(gpsStatus, GPS_TIME_UPDATED, OK);
    gpsTime = gps.time.value();
  }
  if(gps.satellites.isUpdated() && gps.satellites.isValid()){
    bitWrite(gpsStatus, GPS_SATELLITES_UPDATED, OK);
    satellitesNumber = gps.satellites.value();
  }
  /*if(gps.course.isUpdated() && gps.course.isValid()){
    bitWrite(gpsStatus, GPS_COURSE_UPDATED, 1);
    gpsCourse = gps.course.deg();
  }*/
  /*if(gps.courseTo.isUpdated()){
    bitWrite(gpsStatus, GPS_COURSETO_UPDATED, 1);
    Serial.print(gpsCourseTo = gps.courseTo(latitude, longitude, targetLat, targetLng); Serial.print("   ");
  }
  if(gps.distanceBetween.isUpdated() && gps.distanceBetween.isValid()){
    bitWrite(gpsStatus, GPS_DISTANCEBETWEEN_UPDATED, 1);
    Serial.println(gpsDistanceBetween = gps.distanceBetween(latitude, longitude, targetLat, targetLng)); Serial.println("   ");
  }*/
//}

/*void gpsPrintData(){
  if(bitRead(gpsStatus, GPS_STATUS) == OK){
    //Serial.print(gps.charsProcessed() - prevCharsProcessed); Serial.print("   ");
    Serial.print(latitude, 5); Serial.print(" ");
    Serial.print(longitude, 5); Serial.print(" ");
    //Serial.print(gpsCourseTo); Serial.print(" ");
    //Serial.print(gpsDistanceBetween); 
    Serial.print("   ");
    
    Serial.print(gpsAltitude); Serial.print("   ");
    Serial.print(gpsSpeed); Serial.print("   ");
    Serial.print(gpsTime); Serial.print("   ");
    Serial.print(satellitesNumber); Serial.print("   ");
    //Serial.print(gpsCourse); 
    Serial.println("   ");
  }
  /*else if(isGPSok == SLEEP){
    Serial.println("GPS is sleeping");
  }*/
  /*else if(bitRead(gpsStatus, GPS_STATUS) == FAIL){
    Serial.println("GPS failed");
  }
}

void gpsGetDataTask(void *pcParameters){
  while(true){
    gpsGetData();
    gpsPrintData();

    vTaskDelay(refreshRate/portTICK_PERIOD_MS);
  }
}

void gpsFeedObjectTask(void *pvParameters){
  while(true){
    gpsFeedObject();
    vTaskDelay(1);
  }
}*/