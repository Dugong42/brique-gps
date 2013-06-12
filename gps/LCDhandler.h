#include <LiquidCrystal.h>

class LCDhandler {
    private:
        int _ticks;
        bool _isAvailable;
        LiquidCrystal _lcd;

    public:
        LCDhandler();
        void printline(String s, int line);
        void cls();
        void cls(int line);
        void notify(String s);
        void notify(String s, String type);
        void incTicks();
};
