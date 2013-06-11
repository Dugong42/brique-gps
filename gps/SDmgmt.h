#ifndef SDMGMT_H
#define SDMGMT_H

// Définition des codes d'erreur
#define errOpenFile  -1
#define errWrite     -2

#define BUFFER_SIZE 20

class SDmgmt {
  private : 
    char _nameFile[BUFFER_SIZE];
    int _numFile;
    File _logFile;
    
  public :
    SDmgmt();
    int writeCoordinates (long lat, long lon, unsigned long date, unsigned long time, unsigned long gspeed);
    void changeFile(); 
};

#endif
