#ifndef SDMGMT_H
#define SDMGMT_H

// Définition des codes d'erreur
#define errOpenFile  -1
#define errWrite     -2

#define BUFFER_SIZE   4096
#define LOGENTRY_SIZE 128
#define WRITE_DELAY   2000


class SDmgmt {
  private : 
    char _nameFile[BUFFER_SIZE];
    int _numFile;
    File _logFile;
    unsigned long _timerSD;
    
  public :
    SDmgmt();
    int writeCoordinates (long lat, long lon, unsigned long date, unsigned long time, unsigned long gspeed);
    int changeFile(); 
};

#endif
