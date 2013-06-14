#ifndef LCDHANDLER_H
#define LCDHANDLER_H
#include <LiquidCrystal.h>

class LCDhandler {
    private:
        unsigned long _time;
        bool _isAvailable;
        LiquidCrystal _lcd;
        void checkAvailable();

    public:
        LCDhandler();
        void printline(String s, int line);
        void cls();
        void cls(int line);
        void notify(String s);
        void notify(String s, String type);
};
#endif
