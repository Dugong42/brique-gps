/**
 * \file SDhandler.c
 * \brief Fonctions pour sauvegarder des informations sur la carte SD
 * \date 9 Juin 2013
 */

#include <SD.h>
#include "SDhandler.h"

//Constructor
SDhandler::SDhandler() {
    _nameFile[NAMESIZE];
}

// METHOD
// Init SD
int SDhandler::init() {

    // Init SD communication
    if (!SD.begin(CS)) {
        return -3;
    }
    
    strcpy (_nameFile, "GPS0.TXT");
    _logFile = SD.open(_nameFile, FILE_WRITE);

//    //  Initialisation carte
//    if (!SD.exists(".last")) {
//        _lastFile = SD.open(".last", FILE_WRITE);
//        _numFile=0;
//        _lastFile.print(_numFile);
//        strcpy (_nameFile, "GPS0.TXT");
//    }
//
//    else {
//        _lastFile = SD.open(".last", FILE_READ);
//        char buffer[5];
//        int i=-1;
//        do {
//            i++;
//            buffer[i] = _lastFile.read();
////        } while (buffer[i] != '-1');
//        } while (i <= 4);
//        buffer[i]='\0';
//
//        _numFile = (int)strtol(buffer, NULL, 10);
//        _lastFile.close();
//        _numFile = _numFile + 1;
//        sprintf (_nameFile , "GPS%d.TXT", _numFile);
//    }

    return 0;
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
int SDhandler::writeCoordinates (long lat, long lon, unsigned long date,
        unsigned long time, unsigned long gspeed)
{
   // _logFile = SD.open(_nameFile, FILE_WRITE);
    char logEntry[LOGENTRY_SIZE];

    sprintf(logEntry, "%f;%f;%f;%f;%f;", lat, lon, gspeed, date, time);
    if (!_logFile.println (logEntry))
        return errWrite;
    _logFile.flush();
}

/**
 *@fn void SDhandler::changeFileName()
 *@brief Increment the name of the file and create it
 */
int SDhandler::changeFile() {

      _logFile.println("\n");
      _logFile.println("latitude;longitude;date;time;speed;");
      _logFile.flush();
      
//    _numFile = _numFile + 1;
//    sprintf (_nameFile ,"GPS%d.txt", _numFile);
//
//    _lastFile = SD.open(".last", FILE_WRITE);
//    _lastFile.seek(0);
//    _lastFile.print(_numFile, DEC);
//    _lastFile.close();
//
//    _logFile = SD.open(_nameFile, FILE_WRITE);
//    //    _lastFile.seek(0);
//    _logFile.println("latitude;longitude;date;time;speed;");
//    _logFile.close();
//    return 1;

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
        _logFile.close();
        SD.remove(_nameFile);
        _logFile = SD.open(_nameFile, FILE_WRITE);
        lcd.notify("Sync OK.","INFO");
    }
    // if the file isn't open, pop up an error:
    else {
        lcd.notify("FileRead", "ERROR");
    }
}


