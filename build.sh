#!/bin/sh
mkdir -p output/Linux/media
cp ./media/*.png ./output/Linux/media/
cp ./media/*.wav ./output/Linux/media/
cp ./media/*.ttf ./output/Linux/media/
cp ./levels.dat ./output/Linux/levels.dat
gcc *.c -o ./output/Linux/JoppyBricks -lm -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
