#ifndef BLOCKHASH_H
#define BLOCKHASH_H

#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
#include <Magick++.h>
#include "const.h"
#include "log.h"
#include "gpg.h"


using namespace std;
using namespace Magick;

class Blockhash{
	private:
        const int PIXEL_SIZE; // No. of bits required to store value for one color
        const int ROT_DELTA; // Rotating for computing hash with diff. angle
        const char *OUT_PATH; //Intermediate files
        const char *HASH_FILE; //Intermediate hash file before signing
        bool error; //Error flag
        string err_msg; // Error message
        bool set_err(bool error,string err_msg); //Error message setter

    public:
        Blockhash(); // Constructor
        ~Blockhash(); // Destructor
        int process_image(string const &fn,int bits); //Master function
        int print_quantum(Magick::Quantum *pixel_cache,int width,int height); //Print image in RGB format
        int blockhash_int(int bits, Quantum *data, int width, int height, int *hash); //Computing blockhash
        float median(Quantum *data, int n); // Compute median
        int translate_blocks_to_bits(Quantum *blocks, int nblocks, int pixels_per_block,int *hash); // Each block to bit
        int bits_to_hexhash(int *bits, int nbits); // Bits to hexadecimal
        float compare_hash(string const &hash_file1,string const &hash_file2); // Compare two hash files
        string get_err(); //Getter for error message
	};

#endif
