#ifndef SDHANDLER_H
#define SDHANDLER_H

#include <SD.h>
#include "LCDhandler.h"

//Définition de constantes
#define LOGENTRY_SIZE   128
#define NAMESIZE        13

#define MOSI 11
#define MISO 12
#define CLK  13
#define CS 10

class SDhandler {
    private :
        char _nameFile[];
        unsigned long _timerSD;

    public :
        SDhandler();
        void init();
        void writeCoordinates (long lat, long lon, unsigned long date,
                unsigned long time, unsigned long gspeed);
        void changeFile();
        void dumpFile(LCDhandler lcd, char* filename);
};
#endif
