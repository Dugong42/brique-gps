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
const int DELAYNOTIF = 700;

LCDhandler::LCDhandler() : _lcd(
        LiquidCrystal(LCD1, LCD2, LCD3, LCD4, LCD5, LCD6)) {

    // Init LCD size
    _lcd.begin(LCDC, LCDR);
    _isAvailable = true;
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
// Clear a line
void LCDhandler::cls(int line) {
    _lcd.setCursor(0, line);
    for (int i=0; i < LCDC; i++)
        _lcd.print(" ");
}

// PROCEDURE
// Print on a specific line
// Clear end of line
// String lendth must be < LCDC
// Line must is numbered from 0 to LCDR
void LCDhandler::printline(char* s, int line) {
    checkAvailable();

    // Check params
    if (line < LCDR && _isAvailable) {
        _lcd.setCursor(0, line);

        // Allow to print a string < 8 chars
        _lcd.print(s);

        // Clear end of line

        for (int i=0; i < LCDC; i++)
            _lcd.print(" ");

    }
}

void LCDhandler::tprint(unsigned long nb, int line){
    char ch[16]; 
    sprintf(ch,"%lu",nb);
    printline(ch,line);
}

void LCDhandler::tprint( long nb, int line){
    char ch[16]; 
    sprintf(ch,"%ld",nb);
    printline(ch,line);
}

void LCDhandler::tprint( unsigned short nb, int line){
    char ch[16]; 
    sprintf(ch,"%hu",nb);
    printline(ch,line);
}

// PROCEDURE
// Notify user with for a while
// ERR, INFO, WARN
void LCDhandler::notify(char s[16], char type[16]) {
    cls(0);
    _lcd.home();
    _lcd.print(type);
    cls(1);
    _lcd.setCursor(0, 1);
    _lcd.print(s);

    // "A while"
    _isAvailable = false;
    _time = millis();
}

// PROCEDURE
// Notify user with no notification type
void LCDhandler::notify(char s[16]) {
    notify(s, "GPS");
}

