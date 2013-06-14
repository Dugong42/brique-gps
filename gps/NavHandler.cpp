#include "GPShandler.h"
#include "NavHandler.h"
#include "SDhandler.h"

NavHandler::NavHandler(GPShandler & gps) :
  _sdCard(SDhandler())
{
    _mod=0;
    _gps=gps;
    _sdCard.init();
    _writeTimer=millis();
}

unsigned long NavHandler::getAbsoluteDistance() {}
unsigned long NavHandler::getRouteDistance() {}
// Distance between the current location and the previous recorded
unsigned long NavHandler::difference() {}

// Distance between two give points
unsigned long NavHandler::distance(long lat1, long lon1, long lat2, long lon2) {}

void NavHandler::setMod(int mod) { _mod=mod; }

void NavHandler::render() {
    // Do navigation-related work here
    if ( difference()>=1 && millis()-_writeTimer >= WRITE_DELAY) {
        sdWrite();
        _writeTimer=millis();
    }
}



int NavHandler::sdWrite() {
    return _sdCard.writeCoordinates (_gps.getLat(), _gps.getLon(), _gps.getDate(), _gps.getTime(), _gps.getSpeed());
}
