all: main

blockhash.o: blockhash.cpp blockhash.h const.h gpg.h
	c++ $(CFLAGS) -c blockhash.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs` -std=gnu++11

gpg.o: gpg.cpp gpg.h const.h
	c++ $(CFLAGS) -c gpg.cpp -std=gnu++11

main: blockhash.o gpg.o main.cpp
	c++ $(CFLAGS) -o main blockhash.o gpg.o main.cpp `Magick++-config --cppflags --cxxflags --ldflags --libs` -std=gnu++11

clean:
	rm -rf *.o main
