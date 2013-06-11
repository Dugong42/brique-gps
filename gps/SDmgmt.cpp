/**
 * \file SDmgmt.c
 * \brief Fonctions pour sauvegarder des informations sur la carte SD
 * \date 9 Juin 2013
 */

#include <SD.h>
#include "SDmgmt.h"



//Constructor
SDmgmt::SDmgmt(){
  strcpy(_nameFile, "gpslog00.txt");
  _numFile = 00;
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
int SDmgmt::writeCoordinates (long lat, long lon, unsigned long date,
        unsigned long time, unsigned long gspeed)
{
    int n;
    char buffer[100];
    File logFile = SD.open(_nameFile, FILE_WRITE);

    if (logFile) {
        sprintf(buffer, "%f:%f-- %f -- %f:%f", lat, lon, gspeed, date, time);
        if (!logFile.println (buffer))
            return errWrite;
    }
    else
        return errOpenFile;

    logFile.close();
}

/**
*@fn void SDmgmt::changeFileName()
*@brief Increment the name of the file and create it
*/
void SDmgmt::changeFileName() {
  
  _numFile = _numFile + 1;
  sprintf (_nameFile ,"gpslog%d.txt", _numFile);
}

