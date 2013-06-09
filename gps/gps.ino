/*
 * Main file for GPS box management
 *
 * Copyright 2013
 *
 * Alexandre Horréard
 * Julien Nozais
 * Youenn Piolet
 */
// -----------------------------------------------------
// include the library code:
// -----------------------------------------------------

#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include "TinyGPS.h"
#include "SDmgmt.h"

// -----------------------------------------------------
// Definition des constantes
// -----------------------------------------------------
#define RXPIN 3
#define TXPIN 2

// -----------------------------------------------------
// initialize the library with the numbers of the interface pins
// -----------------------------------------------------

LiquidCrystal lcd(4, 5, 6, 7, 8, 9);
SoftwareSerial nss(RXPIN, TXPIN);

// -----------------------------------------------------
// initialiaze variables
// -----------------------------------------------------

TinyGPS gps;
boolean running = false, reset = false;
unsigned long chrono;
boolean bouton1, bouton2, bouton3;
char* nom_aff[5]={"Distance","Position","Vitesse","Batterie","Temps"};
int affichage=0;
const double KNOT_CONV = 0.514444444;

// -----------------------------------------------------
// Fonction d'initialisation
// -----------------------------------------------------

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Print a message to the LCD.
  lcd.print("Init !");
  
   // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   
   if (!SD.begin(10)) {
     lcd.print("Error File");
  }

// -----------------------------------------------------
// Fonction répétée
// -----------------------------------------------------

void loop() {
  // Lecture des pins multiplexés pour les boutons poussoirs
  bouton1 = digitalRead(15);
  bouton2 = digitalRead(16);
  bouton3 = digitalRead(17);

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
    lcd.print(nom_aff[affichage]);
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

