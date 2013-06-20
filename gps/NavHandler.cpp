#include "GPShandler.h"
#include "NavHandler.h"
#include "SDhandler.h"

NavHandler::NavHandler(GPShandler & gps) :
  _sdCard(SDhandler())
{
    _speed=0;
    _path_distance=0;
    _reset=true;
    _start_lat=0;
    _start_lon=0;
    _lat=0;
    _lon=0;
    _lat_p=0;
    _lon_p=0;
    _mod=0;
    _gps=gps;
    _sdCard.init();
    _writeTimer=millis();
}

// WGS84 ellipsoid parameters
const float a = 6378137;
const float b = 6356752.314245;
const float f = 1/(298.257223563);

const float pi = 3.1415926;
const float RAD_CONV = pi/180;
const float PRECISION = 1e-6f;

unsigned long NavHandler::getAbsoluteDistance() {
    return distance_between(_start_lat, _start_lon, _gps.getLat(), _gps.getLon());
}
unsigned long NavHandler::getRouteDistance() { return _path_distance; }

// Distance between the current location and the previous recorded
unsigned long NavHandler::difference() {
    return distance_between(_lat, _lon, _gps.getLat(), _gps.getLon());
}

void NavHandler::reset(){
    _reset=true;
    _path_distance=0;
    _gps.stop();
    _sdCard.changeFile();
}

unsigned long NavHandler::getSpeed(){ return _speed; }

// Distance between two given points
unsigned long NavHandler::distance_between(long lat1, long lon1, long lat2, long lon2){
    
  // Vincenty formula calculates the distance between two points on an ellipsoid
    
    int iteration=0;
    float U1 = atan((1 - f)*tan(lat1*RAD_CONV));
    float U2 = atan((1 - f)*tan(lat2*RAD_CONV));
    float L = abs(lon2-lon1)*RAD_CONV;
    float sinU1=sin(U1), cosU1=cos(U1), sinU2=sin(U2), cosU2=cos(U2);
    
    float lambda = L, lambdaP;
    float sinLambda,cosLambda,sinSigma,cosSigma,sigma,sinAlpha,cosSqAlpha,cos2SigmaM,C;
    do{
        sinLambda = sin(lambda), cosLambda = cos(lambda);
        sinSigma = sqrt((cosU2*sinLambda) * (cosU2*sinLambda) + 
          (cosU1*sinU2-sinU1*cosU2*cosLambda) * (cosU1*sinU2-sinU1*cosU2*cosLambda));
          
        if (sinSigma==0) return 0;  // co-incident points
        
        cosSigma = sinU1*sinU2 + cosU1*cosU2*cosLambda;
        sigma = atan2(sinSigma, cosSigma);
        sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
        cosSqAlpha = 1 - sinAlpha*sinAlpha;
        cos2SigmaM = cosSigma - 2*sinU1*sinU2/cosSqAlpha;
        
        //if (isNaN(cos2SigmaM)) cos2SigmaM = 0;  // equatorial line: cosSqAlpha=0
        
        C = f/16*cosSqAlpha*(4+f*(4-3*cosSqAlpha));
        lambdaP = lambda;
        lambda = L + (1-C) * f * sinAlpha *
          (sigma + C*sinSigma*(cos2SigmaM+C*cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)));
    }while(abs(lambda-lambdaP)>PRECISION && ++iteration<100);
    
    if (iteration==0) return 0;  // formula failed to converge

    float uSq = cosSqAlpha * (a*a - b*b) / (b*b);
    float A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
    float B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));
    float deltaSigma = B*sinSigma*(cos2SigmaM+B/4*(cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)-
      B/6*cos2SigmaM*(-3+4*sinSigma*sinSigma)*(-3+4*cos2SigmaM*cos2SigmaM)));
    float s = b*A*(sigma-deltaSigma);
    
    return (unsigned long)s*1000; //returns distance in mm
}

void NavHandler::setMod(int mod) { _mod=mod; }

void NavHandler::render() {
    // Do navigation-related work here
    unsigned long _diff = difference();
    if ( _diff>=1000 && millis()-_writeTimer >= WRITE_DELAY) {
        sdWrite();
        _lat = _gps.getLat();
        _lon = _gps.getLon();
        _speed = _diff/(millis()-_writeTimer);
        if (_reset){
            _start_lat=_lat;
            _start_lon=_lon;
            _reset = false;
        }else{ //Avoids recording the first position shift
            _path_distance+=_diff;
        }
        _writeTimer=millis();
    }
}



int NavHandler::sdWrite() {
    return _sdCard.writeCoordinates (_gps.getLat(), _gps.getLon(), _gps.getDate(), _gps.getTime(), _gps.getSpeed());
}
