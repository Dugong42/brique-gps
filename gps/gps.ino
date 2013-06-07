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
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Print a message to the LCD.
  lcd.print("Init !");
}

void loop() {
  // Lecture des pins multiplexés pour les boutons poussoirs
  boolean bouton1 = digitalRead(15);
  boolean bouton2 = digitalRead(16);
  boolean bouton3 = digitalRead(17);

  // Fonctions des boutons poussoirs
  if (!bouton1 && !bouton2 && bouton3) {
    lcd.clear();
    lcd.print("Premier");
  } else if (!bouton1 && bouton2 && bouton3){
    lcd.clear();
    lcd.print("Deuxieme");
  } else if (bouton1 && !bouton2 && bouton3) {
    lcd.clear();
    lcd.print("Troisieme");
  } else if (bouton1 && bouton2 && bouton3) {
    lcd.clear();
    lcd.print("Quatrieme");
  }

  delay(100);
}

