#!/bin/sh

if [ ! -f main.ino ]; then
    ln -s gps/gps.ino main.ino
    echo "OK."
    return 0
else
    echo "Already installed."
    return 0
fi
