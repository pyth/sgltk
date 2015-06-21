CC	= g++
CFLAGS	= -g
LFLAGS	= -lGL -lGLEW -lsgltk
BIN	= example

.PHONY: all clean

all: $(BIN)

example: main.cpp gui.cpp gui.h
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $(BIN)

clean:
	rm $(BIN)
