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
#include "SDhandler.h"
#include "TinyGPS.h"
#include "NavHandler.h"

// PINs
const int BUT3  = 17;
const int BUT2  = 16;
const int BUT1  = 15;
const int VOLTPIN = 15;

// Delay to press button to reset
const int RSTDELAY = 2000;

// Delay min between 2 button pressures
const unsigned long DELAYBUTTON = 250;

// Number of cycles to do the refresh rate mean on
const int CYCLE_NUMBER = 100;

// Printed text
char* MSG_MENUS[6]={"Distance", "Position", "Vitesse", "Stats", "LoopRate", "Temps"};

char* MSG_MODS[3]={"Temps","Distance","Combo"};

// Init external components
LCDhandler lcd;
//SDhandler sdCard;
NavHandler nav;

// Declaration of used variables
unsigned long timer;
unsigned long curTimer, prevTimer; // Timers used to do a mean of the gps refresh rate
int meanTick=0;
int affichage;
int writeMod=5;
int voltage;

// FUNCTION
// Initialisation
void setup() {
      
    timer=millis();
    curTimer=millis();
    prevTimer=curTimer;
    pinMode(10, OUTPUT);
    affichage = 1;
        
    //analogReag return an int between 0 and 1023
    voltage = analogRead(VOLTPIN);
    //Affiche en pourcentage si je me rapelle de ma règle de 3
    char batteryLvl[16];
    sprintf(batteryLvl, "%d mv", ((long)voltage*3300)/1023);
        
    // I'm always ready for you bro
    lcd.notify(batteryLvl, "READY");
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
            lcd.tprint(nav.getAbsoluteDistance(), 0);

            // Path distance
            lcd.tprint(nav.getRouteDistance(), 1);
            break;

        case 1 :
            // Show Lat/Long
            lcd.tprint(nav.gps.getLat(), 0);
            lcd.tprint(nav.gps.getLon(), 1);
            break;

        case 2 :
            // Show Speed
            // TODO print unit
            lcd.tprint(nav.getSpeed(),0);
            lcd.printline("cm/s", 1);
            break;

        case 3 :
            // Show info
            // TODO put this in a GPShandler method
            lcd.tprint(nav.gps.getFailed(),0);
            lcd.tprint(nav.gps.getSentences(),1);
            break;

        case 4 :
            // Show Stat
            lcd.printline("Average",0);
            lcd.tprint((curTimer-prevTimer)/CYCLE_NUMBER,1);
            break;

        case 5 :
            // Show Date
            lcd.tprint(nav.gps.getDate(),0);
            lcd.tprint(nav.gps.getTime(),1);
            break;

        default :
            // Error
            lcd.printline("Undef",0);
            lcd.printline("Text",1);
    }
}

// FUNCTION
// Buttons behavior definition.
// Executes a different action depending on the state of the buttons.
void handleButtons() {
    unsigned long rsttimer;
    if (millis()-timer > DELAYBUTTON) {
        switch (demuxButtons()) {
            case 1:
                // Start, Stop, Reset
                rsttimer=millis();

                while (demuxButtons() == 1 && millis()-rsttimer < RSTDELAY) {;}

                if (millis()-rsttimer >= RSTDELAY) {
                    nav.reset();
                    lcd.notify("RESET");

                    //The file on the SD is closed and another one is oponed when the system reset
                    //sdCard.changeFile();

                } else {
                    nav.gps.toggle();
                    nav.gps.isRunning() ? lcd.notify("Running") : lcd.notify("Paused");
                }
                while (demuxButtons() == 1);
                break;

            case 2:
                // Print various information
                affichage = (affichage + 1) % 6;
                lcd.notify(MSG_MENUS[affichage], "MENU");
                break;

            case 3:
                writeMod=writeMod%20+5;
                nav.setMod(writeMod);
                lcd.notify(MSG_MODS[writeMod], "REC BY");
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
        if (demuxButtons() != 0)
            // New timer value
            timer = millis();
    }
}

// This function will loop forever
void loop() {
    handleButtons();
    printInfos();
    nav.render(lcd);

    // Estimate the time spent for CYCLE_NUMBER loops
    meanTick=(meanTick+1)%CYCLE_NUMBER;
    if (meanTick==0) {
        prevTimer=curTimer;
        curTimer=millis();
    }
}

