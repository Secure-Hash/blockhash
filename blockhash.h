#ifndef _BLOCKHASH_H
#define _BLOCKHASH

class Blockhash{
	
	public:
		int process_image(char * fn);
		int print_quantum(Magick::Quantum *pixel_cache,int height,int width);
	};

#endif
