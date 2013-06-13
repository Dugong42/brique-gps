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
    _buffer[BUFFER_SIZE];
}

// METHOD
// Init SD
int SDhandler::init() {
    //  Initialisation carte
    
    if (!SD.exists("lastfile.txt")) {
        _lastFile = SD.open("lastfile.txt", FILE_WRITE);
        _numFile=0;
        _lastFile.write(_numFile);
        strcpy (_nameFile, "gpslog0.txt");
    }
    else {
        _lastFile = SD.open("lastfile.txt", FILE_READ);
        _numFile = _lastFile.read();
        _lastFile.close();
        _numFile = _numFile + 1;
        sprintf (_nameFile ,"gpslog%d.txt", _numFile);
    }

    _logFile = SD.open(_nameFile, FILE_WRITE);
    _timerSD=millis();

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

    char logEntry[LOGENTRY_SIZE];

    if (millis() - _timerSD < WRITE_DELAY) {
        sprintf(logEntry, "%f;%f;%f;%f;%f;\n", lat, lon, gspeed, date, time);
        strcat(_buffer, logEntry);
    }
    else {
        if (!_logFile.println (_buffer))
            return errWrite;
        _timerSD=millis();
        _logFile.flush();
    }


}

/**
 *@fn void SDhandler::changeFileName()
 *@brief Increment the name of the file and create it
 */
int SDhandler::changeFile() {

    _logFile.close();

    _numFile = _numFile + 1;
    sprintf (_nameFile ,"gpslog%d.txt", _numFile);

    _lastFile = SD.open("lastfile", FILE_WRITE);
    _lastFile.seek(0);
    _lastFile.write(_numFile);
    _lastFile.close();

    _logFile = SD.open(_nameFile, FILE_WRITE);
    if (!_logFile.println ("latitude;longitude;date;time;speed;"))
        return errWrite;
    return 1;

}



