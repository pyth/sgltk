CC	= g++
CFLAGS	= -g -std=c++0x
LFLAGS	= -lGL -lGLEW -lassimp -lsgltk
BIN	= example
HDR	= gui.h

.PHONY: all clean

all: $(BIN)

example: main.o gui.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $(BIN)

%.o: %.cpp $(HDR)
	$(CC) $(CFLAGS) -c $<

clean:
	rm $(BIN) *.o
