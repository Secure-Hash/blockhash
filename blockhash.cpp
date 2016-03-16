#include <iostream>
#include <cstring>
#include <Magick++.h>
#include "blockhash.h"

using namespace std;
using namespace Magick;
	
int Blockhash::process_image(char *fn)
{
    int i, j;
    size_t width, height;
    int *hash;
    Image image;
    image.read(fn);
    image.type(TrueColorType);
    width = image.size().width();
    height = image.size().height();

    cout<<height<<endl;
    cout<<width<<endl;

    /* For development purpose take small sample */
    Quantum *pixel_cache = image.getPixels(0,0,5,5);
    print_quantum(pixel_cache,5,5);
    return 0;
}

int Blockhash::print_quantum(Quantum *pixel_cache,int height,int width){
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			cout<<*pixel_cache<<"  "; //Red
			pixel_cache++;
			cout<<*pixel_cache<<"  "; //Green
			pixel_cache++;
			cout<<*pixel_cache<<"  "; //Blue
			pixel_cache++;
			}
			cout<<endl;
		}
	return 0;
	}
