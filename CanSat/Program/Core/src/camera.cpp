#include "globalVars.h"

#include "camera.h"
#include "gpioExtender.h"
#include "myBNO.h"
#include "temperature.h"

bool Camera::setup(bool verbose){
    if(verbose) Serial.println("---Camera setup-----------------------------------");

    if(!softwareReset){
        toggleOnOff(false);
    }

    return true;
}

void Camera::printStatus(){
    Serial.print("Camera: ");
    if(status == Status::status_OFF){
        Serial.println("SUSPENDED");
        return;
    }

    if(status == Status::status_ON){
        Serial.println("ON");
        return;
    }

    Serial.println("RECORDING");
}

void Camera::toggleOnOff(bool mode){

    if(mode == false && status == Status::status_REC){
        toggleRec();
    }

    pca.write(0, mode);
    pca.write(1, mode);

    delay(200);
    if(mode){
        pca.write(2, 1);
        delay(200);
        pca.write(2, 0);

        status = Status::status_ON;
        return;
    }
    status = Status::status_OFF;
}

void Camera::toggleRec(){

    if(status == Status::status_OFF) return;

    pca.write(2, 1);
    delay(200);
    pca.write(2, 0);

    if(status == Status::status_REC) status = Status::status_ON;
    else status = Status::status_REC;
}

void Camera::seekLaunch(){
    static uint8_t step = 0;
    if(!launched){
        switch(step){
            case 0:
                if(bno.accel.z <= -10 || bme.startingPressure - bme.pressure >= 50){
                    launched = true;
                    toggleOnOff(true);
                    step++;
                }
            break;

            case 1:
                if(status != Status::status_REC){
                    toggleRec();
                }
                step++;
                launched = true;
            break;     
        }
    }
}