/*
 * Main file for GPS box management
 *
 * Copyright 2013
 *
 * Alexandre Horréard
 * Julien Nozais
 * Youenn Piolet
 */

// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SD.h>
#include <SoftwareSerial.h>

#include "SDmgmt.h"
#include "GPShandler.h"
#include "TinyGPS.h"

// PINs
const int BUT3  = 17;
const int BUT2  = 16;
const int BUT1  = 15;
const int LCD6  = 9;
const int LCD5  = 8;
const int LCD4  = 7;
const int LCD3  = 6;
const int LCD2  = 5;
const int LCD1  = 4;
const int RXPIN = 3;
const int TXPIN = 2;

// LCD Settings
const int LCDCO = 8;
const int LCDRO = 2;

// Delay to press button to reset
const int RSTDELAY = 2000;

// Nautical mile per hour (knot) in meters per second (m/s)
const double KNOT_CONV = 0.514444444;

// Printed text
const char* MSG_MENUS[5]={"Distance", "Position", "Vitesse", "Batterie", "Temps"};
const char* MSG_INIT="Ready";

// Init external components
LiquidCrystal lcd(LCD1, LCD2, LCD3, LCD4, LCD5, LCD6);
GPShandler gps;
int affichage;

// FUNCTION
// Initialisation
void setup() {

    // set up the LCD's number of columns and rows:
    lcd.begin(LCDCO, LCDRO);

    // Print a message to the LCD.
    lcd.print(MSG_INIT);
    delay(1000);

    //voir exemple arduino SD pour la raison de cette ligne
    pinMode(10, OUTPUT);

    //Initialisation carte
    if (!SD.begin(10)) {
        lcd.print("Error File");
        delay(1000);
    }

    affichage = 1;
}

// FUNCTION
// Button status detection.
//
// Returns the pressed button ID,
// Returns 0 when no or more than 1 button is pushed,
// Returns -1 if something goes wrong
int demuxButtons() {
    boolean buttons[3];
    buttons[0] = digitalRead(BUT1);
    buttons[1] = digitalRead(BUT2);
    buttons[2] = digitalRead(BUT3);

    int status=-1;
    if (!buttons[0] && !buttons[1] && buttons[2])
        status=1;
    else if (!buttons[0] &&  buttons[1] && buttons[2])
        status=2;
    else if ( buttons[0] && !buttons[1] && buttons[2])
        status=3;
    else if ( buttons[0] &&  buttons[1] && buttons[2])
        status=4;
    else
        status=0;
    return status;
}

// FUNCTION
// Print various information
// Latitude, Longitude, Date, Speed, etc.
void printInfos() {
    switch (affichage) {
        case 0 :
            // TODO
            break;

        case 1 :
            // Show Lat/Long
            lcd.clear();
            lcd.print(String(gps.getLat())); // Must use info object
            lcd.setCursor(0,1);
            lcd.print(String(gps.getLon())); // Must use info object
            break;

        case 2 :
            // Show Speed
            lcd.clear();
            lcd.print(String(gps.getSpeed())); // Must use info object
            lcd.setCursor(0,1);
            lcd.print("m/s");
            break;

        case 3 :
            // TODO
            break;

        case 4 :
            // Show Date
            lcd.clear();
            lcd.print(String(gps.getDate())); // Must get info from object
            break;

        default :
            // Error
            lcd.clear();
            lcd.print("Undef info");
    }
}

// FUNCTION
// Buttons behavior definition.
// Executes a different action depending on the state of the buttons.
void handleButtons() {
    unsigned long timer;

    switch (demuxButtons()) {
        case 1:
            // Start, Stop, Reset
            timer=millis();

            // TODO: Make sure this line works and has the correct behavior
            while (demuxButtons() == 1 && millis()-timer < RSTDELAY) {;}

            if (millis()-timer >= RSTDELAY) {
                gps.stop();
                lcd.clear();
                lcd.print("Reset");
                delay(1000);
                while (demuxButtons() == 1) {;}
            } else {
                gps.toggle();
                lcd.clear();
                gps.isRunning() ? lcd.print("Running") : lcd.print("Paused");
                delay(250);
            }
            break;

        case 2:
            // Print various information
            affichage = (affichage+1)%4;
            lcd.clear();
            lcd.print(MSG_MENUS[affichage]);
            delay(250);
            break;

        case 3:
            // TODO Changing recording mode
            lcd.clear();
            lcd.print("SCREEN 3");
            break;

        case 4:
            // TODO PC Transfert
            lcd.clear();
            lcd.print("SCREEN 4");
            break;

        case 0:
            // Nothing to do if nothing is pressed
            break;

        default:
            // Error
            lcd.print("Invalid Button state");
    }
}

// This function will loop forever
void loop() {

    handleButtons();
    gps.refreshData();
    printInfos();

    // Delay between loops
    // Remember that there is an other delay after a button is pressed
    delay(100);
}

