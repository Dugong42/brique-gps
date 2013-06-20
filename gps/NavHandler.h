#ifndef NAVHANDLER_H
#define NAVHANDLER_H
#include "GPShandler.h"
#include "SDhandler.h"

class NavHandler {
    private:
        SDhandler _sdCard;

        bool _reset;
        int _mod,_write_delay,_write_range;
        long _speed_lat, _speed_lon, _lat_p, _lon_p;
        unsigned long _writeTimer;
        long _start_lat, _start_lon;
        long _rec_lat, _rec_lon;
        unsigned long _path_distance;
        unsigned long _speed;
        unsigned long _speedTimer;
        int _write_space;

        unsigned long distance_between(long lat1, long lat2, long lon1, long lon2);
        int sdWrite();


    public:
        NavHandler();

        GPShandler gps;

        void reset();

        unsigned long getSpeed();
        unsigned long getAbsoluteDistance();
        unsigned long getRouteDistance();
        void setMod(int mod);
        void render(LCDhandler &);
};
#endif
