#include "GPShandler.h"
#include "NavHandler.h"
#include "SDhandler.h"

NavHandler::NavHandler(GPShandler & gps) :
  _sdCard(SDhandler())
{
    _lat=0;
    _lon=0;
    _lat_p=0;
    _lon_p=0;
    _mod=0;
    _gps=gps;
    _sdCard.init();
    _writeTimer=millis();
}

const float a = 6378137;
const float b = 6356752.314245;
const float f = 1/(298.257223563);
const float pi = 3.1415926;
const int ITER_LIMIT = 100;
const float RAD_CONV = pi/180;

unsigned long NavHandler::getAbsoluteDistance() {}
unsigned long NavHandler::getRouteDistance() {}
// Distance between the current location and the previous recorded
unsigned long NavHandler::difference() {
        
    return 0;
}

unsigned long NavHandler::distance_between(long lat1, long lat2, long lon1, long lon2){
    // En cours de formation
    float U1 = atan((1 - f)*tan(lat1*RAD_CONV));
    float U2 = atan((1 - f)*tan(lat2*RAD_CONV));
    float L = (lon2-lon1)*RAD_CONV;
    float sinU1=sin(U1), cosU1=cos(U1), sinU2=sin(U2), cosU2=cos(U2);
    
    return 0;
}

// Distance between two given points
unsigned long NavHandler::distance(long lat1, long lon1, long lat2, long lon2) {}

void NavHandler::setMod(int mod) { _mod=mod; }

void NavHandler::render() {
    // Do navigation-related work here
    if ( difference()>=1 && millis()-_writeTimer >= WRITE_DELAY) {
        sdWrite();
        _lat_p = _lat;
        _lon_p = _lon;
        _lat = _gps.getLat();
        _lon = _gps.getLon();
        _writeTimer=millis();
    }
}



int NavHandler::sdWrite() {
    return _sdCard.writeCoordinates (_gps.getLat(), _gps.getLon(), _gps.getDate(), _gps.getTime(), _gps.getSpeed());
}
