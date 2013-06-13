#ifndef SDHANDLER_H
#define SDHANDLER_H

// Définition des codes d'erreur
#define errOpenFile  -1
#define errWrite     -2

//Définition de constantes
#define BUFFER_SIZE   4096
#define LOGENTRY_SIZE 128
#define WRITE_DELAY   2000


class SDhandler {
  private : 
    char _nameFile[13];
    int _numFile;
    File _logFile;
    File _lastFile;
    unsigned long _timerSD;
    char buffer[BUFFER_SIZE];
    
  public :
    SDhandler(LCDhandler &);
    int writeCoordinates (long lat, long lon, unsigned long date, unsigned long time, unsigned long gspeed);
    int changeFile(); 
};

#endif
