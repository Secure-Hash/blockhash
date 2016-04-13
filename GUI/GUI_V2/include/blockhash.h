#ifndef _BLOCKHASH_H
#define _BLOCKHASH

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
		const int PIXEL_SIZE;
		const int ROT_DELTA;
		const char *OUT_PATH;
		const char *HASH_FILE;
        bool error;
        string err_msg;
        bool set_err(bool error,string err_msg);
	public:
		Blockhash();
		~Blockhash();
        int process_image(string const &fn,int bits);
		int print_quantum(Magick::Quantum *pixel_cache,int width,int height);
		void blockhash_int(int bits, Quantum *data, int width, int height, int *hash);
		float median(Quantum *data, int n);
		void translate_blocks_to_bits(Quantum *blocks, int nblocks, int pixels_per_block,int *hash);
        int bits_to_hexhash(int *bits, int nbits);
		float compare_hash(string const &hash_file1,string const &hash_file2);
        string get_err();
	};

#endif
