#include "mySD.h"

#define sdCSpin 10



bool sdSetup(){
    SD.begin(sdCSpin);

    return true;
}