#ifndef GPSHANDLER_H
#define GPSHANDLER_H

#include <SoftwareSerial.h>
#include "TinyGPS.h"
#include "LCDhandler.h"

class GPShandler {
    private:
        long _lat;
        long _lon;
        unsigned long _time;
        unsigned long _date;
        unsigned long _speed;
        unsigned long _fixAge;
        unsigned long _chars;
        unsigned short _failed_checksum;
        unsigned short _sentences;
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
        unsigned short getSentences();
        unsigned short getFailed();
        unsigned long getChars();
        void run();
        void stop();
        void toggle();
        void refreshData(LCDhandler &);
        bool isRunning();
};
#endif
