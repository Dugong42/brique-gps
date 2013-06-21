#include <SoftwareSerial.h>
#include "LCDhandler.h"
#include "GPShandler.h"
#include "TinyGPS.h"

// Nautical mile per hour (knot) in meters per second (m/s)
const double KNOT_CONV = 0.514444444;

// GPS Serial Pins
const int RXPIN = 3;
const int TXPIN = 2;

// GPS serial speed
const int BAUDS = 4800;

// Timeout for GPS trame reception
const int TIMEOUT = 700;

// Constructor
GPShandler::GPShandler() : _nss(SoftwareSerial(RXPIN, TXPIN)) {
    // Init
    _lat    =   0;
    _lon    =   0;
    _time   =   0;
    _date   =   0;
    _speed  =   0;
    _failed_checksum = 0;
    _sentences = 0;
    _chars = 0;
    _isRunning  = false;
    _isReceived = false;
    _nss.begin(BAUDS);

    // Elapsed time in ms after value encoding
    _fixAge =   0;
}

// Public getters
long GPShandler::getLat() { return _lat; }
long GPShandler::getLon() { return _lon; }
unsigned long GPShandler::getTime()  { return _time; }
unsigned long GPShandler::getDate()  { return _date; }
unsigned long GPShandler::getSpeed() { return _speed; }
bool GPShandler::isRunning() { return _isRunning; }
unsigned short GPShandler::getFailed() { return _failed_checksum; }
unsigned short GPShandler::getSentences() { return _sentences; }
unsigned long GPShandler::getChars() { return _chars; }

// Status management
void GPShandler::run()    { _isRunning = true;        }
void GPShandler::stop()   { _isRunning = false;       }
void GPShandler::toggle() { _isRunning = !_isRunning; }

// Information refreshing
void GPShandler::refreshData(LCDhandler & lcd) {
    unsigned long timer;

    if (_isRunning && _nss.available()) {
        // Serial Link UP
        timer = millis();
        do {
            // We try to read a full message, handling transmission timeout in
            // case of communication problems.
            int answer = _nss.read();

            // is the message fully received?
            _isReceived = _gps.encode(answer);

            if (_isReceived) {
                _gps.get_position(&_lat, &_lon, &_fixAge);
                // Time format: hhmmsscc
                // Date format: jjmmaa
                _gps.get_datetime(&_date, &_time, &_fixAge);

                // Converting speed
                _speed = _gps.speed() * KNOT_CONV;

                // Stats : nb chars fed to the gps / nb sentences processed / nb failed checksum tests
                _gps.stats(&_chars, &_sentences, &_failed_checksum);
            }
        } while (_nss.available() && !_isReceived && (millis()-timer < TIMEOUT));
        if (millis()-timer > TIMEOUT)
            lcd.notify("Timeout", "ERR");
    }
}
