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

#include "LCDhandler.h"
#include "GPShandler.h"
#include "SDmgmt.h"
#include "TinyGPS.h"

// PINs
const int BUT3  = 17;
const int BUT2  = 16;
const int BUT1  = 15;
const int RXPIN = 3;
const int TXPIN = 2;

// Delay to press button to reset
const int RSTDELAY = 2000;

// Delay a notification message lasts
const int PRINTDELAY = 300;

// Nautical mile per hour (knot) in meters per second (m/s)
const double KNOT_CONV = 0.514444444;

// Printed text
const char* MSG_MENUS[5]={"Distance", "Position", "Vitesse", "Stats", "Temps"};

// Init external components
LCDhandler lcd;
GPShandler gps;
int affichage;
SDmgmt sdCard;

// FUNCTION
// Initialisation
void setup() {

    //voir exemple arduino SD pour la raison de cette ligne
    pinMode(10, OUTPUT);

    //Initialisation carte
//    if (!SD.begin(10)) {
//        lcd.print("Error initialisation SD");
//        delay(PRINTDELAY);
//    }

    affichage = 1;

    // I'm always ready for you bro
    lcd.notify("READY");
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
            // Straight distance
            lcd.printline("?????? m", 0);

            // Path distance
            lcd.printline("?????? m", 1);
            break;

        case 1 :
            // Show Lat/Long
            lcd.printline(String(gps.getLat()), 0);
            lcd.printline(String(gps.getLon()), 1);
            break;

        case 2 :
            // Show Speed
            lcd.printline(String(gps.getSpeed()),0);
            lcd.printline(String("???? m/s"), 1);
            break;

        case 3 :
            // Show info
            // TODO put this in a GPShandler method
            lcd.printline(String(gps.getFailed()),0);
            lcd.printline(String(gps.getSentences())+" "+String(gps.getChars()),1);
            break;

        case 4 :
            // Show Date
            lcd.printline(String(gps.getDate()),0);
            lcd.printline(String(gps.getTime()),1);
            break;

        default :
            // Error
            lcd.printline(String("Undefined"),0);
            lcd.printline(String("information"),1);
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
                lcd.notify("RESET");

                //The file on the SD is closed and another one is oponed when the system reset
                //sdCard.changeFile();

                while (demuxButtons() == 1) {;}
            } else {
                gps.toggle();
                gps.isRunning() ? lcd.notify("Running") : lcd.notify("Paused");
            }
            break;

        case 2:
            // Print various information
            affichage = (affichage + 1) % 5;
            lcd.notify(MSG_MENUS[affichage], "MENU");
            break;

        case 3:
            // TODO Changing recording mode
            // TODO Explain this
            lcd.notify("REC MODE");
            break;

        case 4:
            // TODO PC Transfert
            lcd.notify("USB SYNC");
            break;

        case 0:
            // Nothing to do if nothing is pressed
            break;

        default:
            // Error
            lcd.notify("Invalid Button state", "ERR");
    }
}

// This function will loop forever
void loop() {

    gps.countTick();
    handleButtons();
    printInfos();
    gps.refreshData();
    //Assuming we write everything, data is logged here
    //TODO Do we write everything ? If no, what do we check ?
    //sdCard.writeCoordinates (gps.getLat(), gps.getLon(), gps.getDate(), gps.getTime(), gps.getSpeed());
}

