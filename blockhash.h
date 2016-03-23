#ifndef _BLOCKHASH_H
#define _BLOCKHASH

#include <Magick++.h>

using namespace std;
using namespace Magick;

class Blockhash{
	private:
		const int PIXEL_SIZE;
	public:
		Blockhash();
		~Blockhash();
		int process_image(char * fn,int bits, int quick, int debug);
		int print_quantum(Magick::Quantum *pixel_cache,int width,int height);
		void blockhash_int(int bits, Quantum *data, int width, int height, int **hash);
	};

#endif
