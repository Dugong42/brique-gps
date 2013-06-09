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
#include <SoftwareSerial.h>
#include <SD.h>

#include "TinyGPS.h"
#include "SDmgmt.h"

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
const int LCDCO = 16;
const int LCDRO =  2;

// Nautical mile per hour (knot) in meters per second (m/s)
const double KNOT_CONV = 0.514444444;

// Printed text
const char* MSG_MENUS[5]={"Distance", "Position", "Vitesse", "Batterie", "Temps"};
const char* MSG_INIT="GPS Ready.";

// initialization of components with correct pins
LiquidCrystal lcd(LCD1, LCD2, LCD3, LCD4, LCD5, LCD6);
SoftwareSerial nss(RXPIN, TXPIN);
TinyGPS gps;

// Global variables init

// Initialisation
void setup() {
    // set up the LCD's number of columns and rows:
    lcd.begin(LCDCO, LCDRO);

    // Print a message to the LCD.
    lcd.print(MSG_INIT);

    //voir exemple arduino SD pour la raison de cette ligne
    pinMode(10, OUTPUT);

    //Initialisation carte
    if (!SD.begin(10)) {
        lcd.print("Error File");
    }
}

// This function will loop forever
void loop() {

    // Variables
    boolean bouton1, bouton2, bouton3;
    boolean running=false, reset=false;
    unsigned long chrono;
    int affichage=0;

    // Lecture des pins multiplexés pour les boutons poussoirs
    bouton1 = digitalRead(BUT1);
    bouton2 = digitalRead(BUT2);
    bouton3 = digitalRead(BUT3);

    // -----------------------------------------------------
    // Fonctions des boutons poussoirs
    // -----------------------------------------------------

    // Premier bouton : Start/Stop/Reset
    if (!bouton1 && !bouton2 && bouton3) {

        // Attente du reset
        chrono = millis();
        while (!bouton1 && !bouton2 && bouton3 && millis()-chrono < 2000){
            bouton1 = digitalRead(15);
            bouton2 = digitalRead(16);
            bouton3 = digitalRead(17);
        }

        // Action en fonction du timer
        if (millis()-chrono >= 2000) {
            reset = true;
            running = false;
            lcd.clear();
            lcd.print("Reset");
        } else {
            running = !running;
            lcd.clear();
            running ? lcd.print("Start") : lcd.print("Stop");
        }

        // Bouton 2 : Boucle sur des informations diverses
    } else if (!bouton1 && bouton2 && bouton3){

        affichage = (affichage+1)%4;
        lcd.clear();
        lcd.print(MSG_MENUS[affichage]);
        delay(250);

        // Bouton 3 : Changement de mode d'enregistrement
    } else if (bouton1 && !bouton2 && bouton3) {
        lcd.clear();
        lcd.print("Troisieme");

        // Bouton 4 : Transfert PC
    } else if (bouton1 && bouton2 && bouton3) {
        lcd.clear();
        lcd.print("Quatrieme");
    }

    // -----------------------------------------------------
    // Corps du code à réaliser en fonction du bouton pressé
    // et / ou de l'état actuel
    // -----------------------------------------------------

    // fix_age est le temps en ms depuis que la valeur a été encodée
    long lat=0, lon=0;
    unsigned long fix_age, time=0, date=0, gspeed=0;
    boolean trame_recue=false;

    if (running){
        // Reception de la liaison serie vers l'objet GPS
        if (nss.available()){
            // La structure do..while essaye de lire une trame complète pour
            // éviter de perdre des trames si on attendait trop longtemps de vider
            // le buffer série, et gère le blocage si la trame n'arrive jamais
            chrono = millis();
            do {
                int c = nss.read();
                // Si une trame est entièrement reçue
                trame_recue = gps.encode(c);
                if (trame_recue){
                    gps.get_position(&lat, &lon, &fix_age);
                    // Temps en format hhmmsscc, date en format jjmmaa
                    gps.get_datetime(&date, &time, &fix_age);
                    // Vitesse en 100e de noeud -> m/s
                    gspeed = gps.speed()*KNOT_CONV;
                }
                writeCoordinates(lat, lon, date, time, gspeed);
            } while (nss.available() && !trame_recue && millis()-chrono<1000);
        }
    }

    switch (affichage){
        case 0 :
            break;

        case 1 :
            lcd.clear();
            lcd.print(String(lat));
            lcd.setCursor(0,1);
            lcd.print(String(lon));
            break;

        case 2 :
            lcd.clear();
            lcd.print(String(gspeed));
            lcd.setCursor(0,1);
            lcd.print("m/s");
            break;

        case 3 :
            break;

        case 4 :
            lcd.clear();
            lcd.print(String(date));
            break;

        default :
            lcd.clear();
            lcd.print("Ta chatte");
    }
    delay(100);
}

