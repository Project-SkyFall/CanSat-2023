#include "globalVars.h"
#include "myBme.h"

bool MyBme::setup(){
    Serial.println("---BME setup---");

    if(!bme.begin(0x76, &Wire)) return false;

    return true;
}

void MyBme::getData(){
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" °C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    /*Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");*/

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}

void bmeGetData(void *pvParameters){
    while(true){
       bme.getData();
       vTaskDelay(refreshRate/portTICK_PERIOD_MS); 
    }
}