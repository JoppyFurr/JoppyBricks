#!/bin/sh
mkdir -p output/Windows/media
cp ./dll/* ./output/Windows/
cp ./media/*.png ./output/Windows/media/
cp ./media/*.wav ./output/Windows/media/
cp ./media/*.ttf ./output/Windows/media/
cp ./levels.dat ./output/Windows/levels.dat
i486-mingw32-gcc *.c -o ./output/Windows/JoppyBricks.exe -lm -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

