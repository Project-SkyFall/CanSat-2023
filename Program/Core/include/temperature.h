#ifndef MYBME_H
#define MYBME_H

#include <Adafruit_BME280.h>
#include <DallasTemperature.h>

class MyBme : public Adafruit_BME280{
    public:
    bool setup(bool debug=false);
    void getData();
    void printData();

    byte address;
    byte status;
    float temperature;
    float pressure;
    float humidity;
};

class MyDS18B20 : public DallasTemperature{
    public:
    //MyDS18B20(OneWire*, uint8_t);
    using DallasTemperature::DallasTemperature;
    bool setup(bool verbose=false);
    void getData();
    void printData();

    byte status;
    float temperature;

    private:
    uint8_t _pin;
};

void bmeGetDataTask(void *pvParameters);

extern MyBme bme;
extern MyDS18B20 ds18;

#endif