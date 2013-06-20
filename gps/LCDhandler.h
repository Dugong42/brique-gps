#ifndef LCDHANDLER_H
#define LCDHANDLER_H
#include <LiquidCrystal.h>

class LCDhandler {
    private:
        unsigned long _time;
        bool _isAvailable;
        LiquidCrystal _lcd;
        void checkAvailable();
        void foo(char toto[]);

    public:
        LCDhandler();
        void printline(char* s, int line);
        void cls();
        void cls(int line);
        void notify(char* s);
        void notify(char* s, char* type);
        void tprint(unsigned long nb, int line);
        void tprint( long nb, int line);
        void tprint( unsigned short nb, int line);
};
#endif
