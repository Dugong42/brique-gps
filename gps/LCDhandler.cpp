#include <LiquidCrystal.h>
#include <Arduino.h>
#include "LCDhandler.h"

// LCD Pins
const int LCD1 = 4;
const int LCD2 = 5;
const int LCD3 = 6;
const int LCD4 = 7;
const int LCD5 = 8;
const int LCD6 = 9;

// LCD Size
const int LCDC = 8;
const int LCDR = 2;

// Duration of a notification in ms
const int DELAYNOTIF = 1000;

LCDhandler::LCDhandler() : _lcd(
        LiquidCrystal(LCD1, LCD2, LCD3, LCD4, LCD5, LCD6)) {

    // Init LCD size
    _lcd.begin(LCDC, LCDR);
    _isAvailable = true;
    // Enable autoscroll for strings > LCDC
    //_lcd.autoscroll();
}

// PROCEDURE
// Update lock
void LCDhandler::checkAvailable() {
    if (! _isAvailable) {
        if (millis()-_time >= DELAYNOTIF) {
            _isAvailable = true;
        }
    }
}

// PROCEDURE
// Clear the screen
void LCDhandler::cls() {
    _lcd.home();
}

// PROCEDURE
// Clear a line
void LCDhandler::cls(int line) {
    for (int i=LCDC; i < LCDC; i++)
        _lcd.print(" ");
}

// PROCEDURE
// Print on a specific line
// Clear end of line
// String lendth must be < LCDC
// Line must is numbered from 0 to LCDR
void LCDhandler::printline(String s, int line) {
    checkAvailable();

    // Check params
    if (line < LCDR && _isAvailable) {
        cls(line);
        _lcd.setCursor(0, line);

        // Allow to print a string < 8 chars
        _lcd.print(s);

        // Clear end of line
        int l = s.length();
        if (l < 8) {
            for (int i=l; i < LCDC; i++)
                _lcd.print(" ");
        }
    }
}

// PROCEDURE
// Notify user with for a while
// ERR, INFO, WARN
void LCDhandler::notify(String s, String type) {
    checkAvailable();
    if (_isAvailable) {
        cls();
        printline("[" + type + "]", 0);
        printline(s, 1);

        // "A while"
        _isAvailable = false;
        _time = millis();
    }
}

// PROCEDURE
// Notify user with no notification type
void LCDhandler::notify(String s) {
    notify(s, "GPS");
}

