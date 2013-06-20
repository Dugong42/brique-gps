/**
 * \file SDhandler.c
 * \brief Fonctions pour sauvegarder des informations sur la carte SD
 * \date 9 Juin 2013
 */

#include <SD.h>
#include "SDhandler.h"

//Constructor
SDhandler::SDhandler() {;
}

void SDhandler::init() {
    strcpy(_nameFile, "GPS.TXT");
    SD.begin(CS);

    File logFile = SD.open(_nameFile, FILE_WRITE);
    if (logFile) {
        logFile.println("latitude;longitude;date;time;speed;");
        logFile.close();
    }

}

/**
 *\fn int writeCoordinates (long lat, long lon, long date, long time)
 *\brief Ecrit dans un fichier sur la carte SD les coordonnées
 *	 des points enregistrés et leur time log
 *\param[in] lat Latitude du point
 *\param[in] lon Longitude du point
 *\param[in] date Date de la prise du point
 *\param[in] time Heure de la prise du point
 *\param[in] gspeed
 *\returns 1 si ok
 *	   -1 si ne peut pas ouvrir le fichier
 *	   -2 si erreur ecriture
 */
void SDhandler::writeCoordinates (long lat, long lon, unsigned long date,
        unsigned long time, unsigned long gspeed)
{
    char logEntry[LOGENTRY_SIZE];
    File logFile = SD.open(_nameFile, FILE_WRITE);

    sprintf(logEntry, "%f;%f;%f;%f;%f;", lat, lon, gspeed, date, time);

    if (logFile) {
        logFile.println(logEntry);
        logFile.close();
    }
}

/**
 *@fn void SDhandler::changeFileName()
 *@brief Increment the name of the file and create it
 */
void SDhandler::changeFile() {
    File logFile = SD.open(_nameFile, FILE_WRITE);
    if (logFile) {
        logFile.println("");
        logFile.println("latitude;longitude;date;time;speed;");
        logFile.close();
    }
}

// PROCEDURE
// read a file "filename" to Serial port
void SDhandler::dumpFile(LCDhandler & lcd) {
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File logFile = SD.open(_nameFile, FILE_READ);
    // if the file is available, write to it:
    if (logFile && Serial.available()) {
        while (logFile.available()) {
            lcd.printline("Reading", 0);
            lcd.printline(_nameFile, 1);
            Serial.begin(9600);
            Serial.println(logFile.read());
        }

        logFile.close();
        SD.remove(_nameFile);
        lcd.notify("Sync OK.", "INFO");
    }
    // if the file isn't open, pop up an error:
    else {
        lcd.notify("FileRead", "ERROR");
    }
}


