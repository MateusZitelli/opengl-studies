CC = g++
CFLAGS = -I. -lGL -lglfw -lGLEW -lGLU -std=c++11
DEPS = Configs.h Camera.h Engine.h Light.h Object.h World.h Utils.h 
OBJ = main.o Camera.o Engine.o Light.o Object.o World.o Utils.o 

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ core *~ 
