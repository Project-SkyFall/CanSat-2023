#ifndef GPS_TEMP_H
#define GPS_TEMP_H

#include "TinyGPS++.h"

byte gpsSetup();
void gpsFeedObject();
void gpsGetData();
void gpsPrintData();

extern TinyGPSPlus gps;

#endif