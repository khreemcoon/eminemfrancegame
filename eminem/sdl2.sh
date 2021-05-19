#!/bin/bash

echo "SDL2 g++ shell script"
read -p "Please enter the name of the output file: " of

for cpp in *.cpp; do
	files="$files $cpp"
done


g++ -v $files -lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_ttf -o $of


