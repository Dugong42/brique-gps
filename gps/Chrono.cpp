#include "Chrono.h"

Chrono::Chrono() { _running = false; }

void Chrono::reset()    {   _running = false;       }
void Chrono::start()    {   _running = true;        }
void Chrono::stop()     {   _running = false;       }
void Chrono::toggle()   {   _running = !_running;   }

bool Chrono::isRunning() { return _running; }

