CC=g++
CFLAGS=-I.
hellomake: main.cpp Utils.c
	$(CC) -o main main.cpp Utils.c -I. -lGL -lglfw -lGLEW -lGLU
