#ifndef MYBME_H
#define MYBME_H

#include <Adafruit_BME280.h>

class MyBme : public Adafruit_BME280{
    public:
    bool setup();
    void getData();
};

void bmeGetData(void *pvParameters);

extern MyBme bme;

#endif