#include "GPShandler.h"
#include "LCDhandler.h"
#include "NavHandler.h"
#include "SDhandler.h"

NavHandler::NavHandler() :
    sdCard(SDhandler()),
    gps(GPShandler())

{
    _write_delay=1000;
    _write_space=5;
    _speed=0;
    _path_distance=0;
    _reset=true;
    _start_lat=0;
    _start_lon=0;
    _speed_lat=0;
    _speed_lon=0;
    _rec_lon=0;
    _rec_lat=0;
    _mod=0;
    _writeTimer=millis();
    _speedTimer=_writeTimer;
    setMod(2);
}

// WGS84 ellipsoid parameters
const float a = 6378137;
const float b = 6356752.314245;
const float f = 1/(298.257223563);

const float pi = 3.1415926;
const float RAD_CONV = pi/180;
const float LIMIT = 1e-9f;

const int PRECISION = 1000; // The distance is rounded to 1m*PRECISION

// Getters and setters

unsigned long NavHandler::getAbsoluteDistance() {
    //return distance_between(4826978, 406634, 4827024, 406554);
    return distance_between(_start_lat, _start_lon, gps.getLat(), gps.getLon());
}
unsigned long NavHandler::getRouteDistance() { return _path_distance; }

unsigned long NavHandler::getSpeed(){ return _speed; }

void NavHandler::setMod(int mod) { _write_space=mod; }

void NavHandler::reset(){
    _reset=true;
    _path_distance=0;
    _start_lat=0;
    _start_lon=0;
    gps.stop();
    sdCard.changeFile();
}

// Other functions

void NavHandler::render(LCDhandler & lcd) {

    unsigned long lat,lon;

    gps.refreshData(lcd);

    bool newCoord = _speed_lat!=gps.getLat() || _speed_lon!=gps.getLon();

    // Do navigation-related work here
    if (gps.isRunning() && newCoord){
      lat=gps.getLat();
      lon=gps.getLon();
      unsigned long _speed_diff = distance_between(_speed_lat, _speed_lon, lat, lon);
      unsigned long _rec_diff = distance_between(_rec_lat, _rec_lon, lat, lon);

      // Enregistrement de coordonnées

      if ( _rec_diff>=_write_space*PRECISION && millis()-_writeTimer >= _write_delay) {
          _rec_lat = lat;
          _rec_lon = lon;

          sdWrite();
          _writeTimer=millis();
          if(!_reset) _path_distance+=_rec_diff;
      }

      // Calcul de vitesse instantanée
      if (millis()-_speedTimer>=2000){
        _speed = _speed_diff*1000/(millis()-_speedTimer);
        _speedTimer=millis();
        _speed_lat=lat;
        _speed_lon=lon;
      }

      // Mémorisation du start
      if (_reset){
          _reset = false;
          _start_lat=lat;
          _start_lon=lon;
      }
    }
}

void NavHandler::sdWrite() {
    //return 0;
    sdCard.writeCoordinates (gps.getLat(), gps.getLon(), gps.getDate(), gps.getTime(), gps.getSpeed());
}

// Distance between two given points
unsigned long NavHandler::distance_between(long lat1, long lon1, long lat2, long lon2){

    // Vincenty formula calculates the distance between two points on an ellipsoid

    int iteration=0;
    float U1 = atan((1 - f)*tan(lat1*RAD_CONV*1e-5));
    float U2 = atan((1 - f)*tan(lat2*RAD_CONV*1e-5));
    float L = (lon2-lon1)*RAD_CONV*1e-5;
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

        if (cosSqAlpha==0) { cos2SigmaM = 0; } // equatorial line: cosSqAlpha=0
        else{ cos2SigmaM = cosSigma - 2*sinU1*sinU2/cosSqAlpha; }

        C = f/16*cosSqAlpha*(4+f*(4-3*cosSqAlpha));
        lambdaP = lambda;
        lambda = L + (1-C) * f * sinAlpha *
            (sigma + C*sinSigma*(cos2SigmaM+C*cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)));
    } while(abs(lambda-lambdaP)>LIMIT && ++iteration<100);

    if (iteration==100) return 0;  // formula failed to converge

    float uSq = cosSqAlpha * (a*a - b*b) / (b*b);
    float A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
    float B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));
    float deltaSigma = B*sinSigma*(cos2SigmaM+B/4*(cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)-
                B/6*cos2SigmaM*(-3+4*sinSigma*sinSigma)*(-3+4*cos2SigmaM*cos2SigmaM)));
    float s = b*A*(sigma-deltaSigma);

    return (unsigned long)(s*PRECISION); //returns distance in m * 100
}

void NavHandler::initUSBSync(LCDhandler & lcd) {
    sdCard.dumpFile(lcd);
}
