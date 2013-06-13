// Définition des codes d'erreur
#define errOpenFile  -1
#define errWrite     -2

//Définition de constantes
#define BUFFER_SIZE     4096
#define LOGENTRY_SIZE   128
#define WRITE_DELAY     2000
#define NAMESIZE        13
class SDhandler {
    private :
        char _nameFile[];
        int _numFile;
        File _logFile;
        unsigned long _timerSD;
        char _buffer[];

    public :
        SDhandler();
        int init();
        int writeCoordinates (long lat, long lon, unsigned long date, unsigned long time, unsigned long gspeed);
        int changeFile();
};
