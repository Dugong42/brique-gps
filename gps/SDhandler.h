#ifndef SDHANDLER_H
#define SDHANDLER_H

#include <SD.h>
#include "LCDhandler.h"
// Définition des codes d'erreur
#define errOpenFile  -1
#define errWrite     -2

//Définition de constantes
#define LOGENTRY_SIZE   128
#define WRITE_DELAY     2000
#define NAMESIZE        13

#define MOSI 11
#define MISO 12
#define CLK  13
#define CS 10

class SDhandler {
    private :
        char _nameFile[];
        int _numFile;
        File _logFile;
        File _lastFile;
        unsigned long _timerSD;

    public :
        SDhandler();
        int init();
        int writeCoordinates (long lat, long lon, unsigned long date,
                unsigned long time, unsigned long gspeed);
        int changeFile();
        void dumpFile(LCDhandler lcd, char* filename);
};
#endif
