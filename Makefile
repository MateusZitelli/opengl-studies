CC=g++
CFLAGS=-I.
hellomake: main.cpp Utils.cpp Object.cpp
	$(CC) -o main main.cpp Utils.cpp Object.cpp -I. -lGL -lglfw -lGLEW -lGLU
