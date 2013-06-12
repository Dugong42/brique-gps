#include <LiquidCrystal.h>

class LCDhandler {
    private:
        LiquidCrystal _lcd;

    public:
        LCDhandler();
        void printline(String s, int line);
        void cls();
        void cls(int line);
        void notify(String s);
        void notify(String s, String type);
};
