#ifndef NAVHANDLER_H
#define NAVHANDLER_H
#include "GPShandler.h"
#include "SDhandler.h"

#define WRITE_DELAY 2000

class NavHandler {
    private:
        SDhandler _sdCard;
        GPShandler _gps;
        
        
        int _mod;
        long _lat, _lon, _lat_p, _lon_p;
        unsigned long _writeTimer;
        
        
        
        unsigned long difference();
        unsigned long distance(long lat1, long lon1, long lat2, long lon2);
        unsigned long distance_between(long lat1, long lat2, long lon1, long lon2);
        int sdWrite();
        
        
    public:
        NavHandler(GPShandler &);
        
        unsigned long getAbsoluteDistance();
        unsigned long getRouteDistance();
        void setMod(int mod);
        void render();
};
#endif
