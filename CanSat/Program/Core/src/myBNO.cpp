#include "globalVars.h"

MyIMU::MyIMU(){

}

bool MyIMU::setup(bool verbose){
    verbose ? Serial.println("---DS18B20 setup----------------------------------") : 0;
    
    return true;
}