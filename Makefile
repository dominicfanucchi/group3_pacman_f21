all: dfanucchi

dfanucchi: dfanucchi.cpp
	g++ dfanucchi.cpp -Wall -Wextra -o test

clean:
	rm -f test
