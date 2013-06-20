/**
 * \file SDhandler.c
 * \brief Fonctions pour sauvegarder des informations sur la carte SD
 * \date 9 Juin 2013
 */

#include <SD.h>
#include "SDhandler.h"

//Constructor
SDhandler::SDhandler() {
    strcpy(_nameFile, "GPS.TXT");

    SD.begin(CS);
}

// METHOD
// Init SD
void SDhandler::init() {
;
    // Init SD communication
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
    // _logFile = SD.open(_nameFile, FILE_WRITE);
    char logEntry[LOGENTRY_SIZE];

    sprintf(logEntry, "%f;%f;%f;%f;%f;\n", lat, lon, gspeed, date, time);
    _logFile = SD.open(_nameFile, FILE_WRITE);

    if (_logFile) {
        _logFile.println(logEntry);
        _logFile.close();
    }
}

/**
 *@fn void SDhandler::changeFileName()
 *@brief Increment the name of the file and create it
 */
void SDhandler::changeFile() {
    _logFile = SD.open(_nameFile, FILE_WRITE);
    if (_logFile) {
        _logFile.println("\n");
        _logFile.println("latitude;longitude;date;time;speed;\n");
        delay(4000);
        _logFile.close();
    }
}

// PROCEDURE
// read a file "filename" to Serial port
void SDhandler::dumpFile(LCDhandler lcd, char* filename) {
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.

    // if the file is available, write to it:
    if (_logFile) {
        while (_logFile.available()) {
            lcd.printline("Reading",0);
            lcd.printline(filename,1);
            Serial.write(_logFile.read());
        }
    //    _logFile.close();
        SD.remove(_nameFile);
    //    _logFile = SD.open(_nameFile, FILE_WRITE);
        lcd.notify("Sync OK.","INFO");
    }
    // if the file isn't open, pop up an error:
    else {
        lcd.notify("FileRead", "ERROR");
    }
}


