CC=g++
CFLAGS=-I.
hellomake: main.cpp Utils.cpp Object.cpp
	$(CC) -o main-phong main.cpp Utils.cpp Object.cpp -I. -lGL -lglfw -lGLEW -lGLU
