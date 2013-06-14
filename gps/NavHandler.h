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
        unsigned long _writeTimer;
        unsigned long difference();
        unsigned long distance(long lat1, long lon1, long lat2, long lon2);
        int sdWrite();
    public:
        NavHandler(GPShandler &);
        
        unsigned long getAbsoluteDistance();
        unsigned long getRouteDistance();
        
        void setMod(int mod);
        void render();
};
#endif
