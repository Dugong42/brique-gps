#include <SoftwareSerial.h>
#include "TinyGPS.h"

class GPShandler {
    private:
        long _lat;
        long _lon;
        unsigned long _time;
        unsigned long _date;
        unsigned long _speed;
        unsigned long _fixAge;
        bool _isReceived;
        bool _isRunning;
        SoftwareSerial _nss;
        TinyGPS _gps;


    public:
        GPShandler();
        long getLat();
        long getLon();
        unsigned long getTime();
        unsigned long getDate();
        unsigned long getSpeed();
        void run();
        void stop();
        void toggle();
        void refreshData();
};
