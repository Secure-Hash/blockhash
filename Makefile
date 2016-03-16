all: main

blockhash.o: blockhash.cpp blockhash.h
	c++ -c blockhash.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs`

main: blockhash.o main.cpp
	c++ -o main blockhash.cpp main.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs`

clean:
	rm -rf *.o main
