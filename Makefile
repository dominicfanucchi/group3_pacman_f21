CFLAGS = -I ./include
##LIB    = ./lib/fmod/libfmodex64.so
LFLAGS = -lrt -lX11 -lGLU -lGL -lm #-lXrandr

all: snake

snake: snake.cpp log.cpp dfanucchi.cpp kwood.cpp anguyen2.cpp
	g++ $(CFLAGS) snake.cpp log.cpp dfanucchi.cpp kwood.cpp anguyen2.cpp libggfonts.a \
	/usr/lib/x86_64-linux-gnu/libopenal.so \
	/usr/lib/x86_64-linux-gnu/libalut.so \
	-Wall -Wextra $(LFLAGS) -o snake

clean:
	rm -f snake
	rm -f *.o

