#include <iostream>
#include <cstring>
#include <cmath>
#include "blockhash.h"


Blockhash::Blockhash():PIXEL_SIZE(3)
{
}

Blockhash::~Blockhash()
{
}
	
int Blockhash::process_image(char *fn, int bits, int quick, int debug)
{
    int i, j;
    size_t width, height;
    int *hash;
    Image image;
    image.read(fn);
    image.type(TrueColorType);
    width = image.size().width();
    height = image.size().height();

    /* For development purpose take small sample */
    Quantum *pixel_cache = image.getPixels(0,0,width,height);
    hash = (int *)malloc(bits * bits * sizeof(int));
    blockhash_int(bits, pixel_cache, width, height, &hash);
    //print_quantum(pixel_cache,width,height);
    return 0;
}

int Blockhash::print_quantum(Quantum *pixel_cache,int width,int height){
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


void Blockhash::blockhash_int(int bits, Quantum *data, int width, int height, int **hash)
{
    int    i, x, y, ix, iy,j;
    int    ii;
    int    block_width;
    int    block_height;
    Quantum  *blocks;
    Quantum  value;

    block_width = width / bits;
    block_height = height / bits;
    cout<<"[DEBUG] Height: "<<height<<", Width: "<<width<<endl;
	cout<<"[DEBUG] Block Height: "<<block_height<<", Block Width: "<<block_width<<endl;

	cout.setf(ios::fixed, ios::floatfield);
	cout.setf(ios::showpoint);
	cout.precision(1);

    blocks = (Quantum *)calloc(bits * bits, sizeof(Quantum));
    for (y = 0; y < bits; y++) {
        for (x = 0; x < bits; x++) {
            value = 0;
            j=0;
            for (iy = 0; iy < block_height; iy++) {
                for (ix = 0; ix < block_width; ix++) {
                    ii = ((y * block_height + iy) * width + (x * block_width + ix)) * 3; // 4 for RGBA
					value += data[ii] + data[ii+1] + data[ii+2]; 
                }
            }
            blocks[y * bits + x] = value;
            cout<<value<<"  ";
        }
        cout<<endl;
    }
}
