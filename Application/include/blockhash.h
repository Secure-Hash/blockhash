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
#include <QObject>

using namespace std;
using namespace Magick;

class Blockhash : public QObject{
    Q_OBJECT
	private:
        const int PIXEL_SIZE; // No. of bits required to store value for one color
        const int ROT_DELTA; // Rotating for computing hash with diff. angle
        const char *OUT_PATH; //Intermediate files
        const char *HASH_FILE; //Intermediate hash file before signing
        const int NUM_ROT;
        bool error; //Error flag
        string err_msg; // Error message
        int result;
        int progress; //Current progress
        string img_path;
        int num_hash_bits;

        bool set_err(bool error,string err_msg); //Error message setter
        int set_progress(int progress); //Setter for progress
        int blockhash(int bits, Quantum *data, int width, int height, int *hash); //Computing blockhash
        float median(Quantum *data, int n); // Compute median
        int translate_blocks_to_bits(Quantum *blocks, int nblocks, int pixels_per_block,int *hash); // Each block to bit
        int bits_to_hexhash(int *bits, int nbits); // Bits to hexadecimal
        int print_quantum(Magick::Quantum *pixel_cache,int width,int height); //Print image in RGB format
        int compute_hash(string const &fn,int bits); //Master function

    public:
        Blockhash(); // Constructor
        int init_compute_hash(string const &fn, int bits);
        float compare_hash(string const &hash_file1,string const &hash_file2); // Compare two hash files
        string get_err(); //Getter for error message
        int get_progress(); //Getter for progress
        int get_result();

    signals:
        void progressValueChanged(int);
        void progressRangeChanged(int,int);
        void finished();

    public slots:
        void compute_hash();
	};

#endif
