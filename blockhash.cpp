#include <iostream>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include "blockhash.h"

static int cmpfloat(const void *pa, const void *pb)
{
    float a = *(const float *) pa;
    float b = *(const float *) pb;
    return (a < b) ? -1 : (a > b);
}

Blockhash::Blockhash():PIXEL_SIZE(CPIXEL_SIZE), ROT_DELTA(CROT_DELTA)
{
}

Blockhash::~Blockhash()
{
}

float Blockhash::median(Quantum *data, int n)
{
    Quantum *sorted;
    float result;

    sorted = (Quantum *)malloc(n * sizeof(Quantum));
    memcpy(sorted, data, n * sizeof(Quantum));
    qsort(sorted, n, sizeof(Quantum), cmpfloat); //Assuming Quantum as float, make it installation independent

    if (n % 2 == 0) {
        result = (float) (sorted[n / 2] + sorted[n / 2 + 1]) / 2;
    } else {
        result = (float) sorted[n / 2];
    }

    free(sorted);
    return result;
}

int Blockhash::process_image(char *fn, int bits, int quick, int debug)
{
    int i, j;
    size_t width, height;
    int *hash;
    char name[10];
    Image image;
    Image dup;
    image.read(fn);
    image.modifyImage();
    image.type(TrueColorType);
    for(i=0;i<=5;i++){
		dup = image;
		dup.modifyImage();
		dup.rotate(i*ROT_DELTA); //Rotation with 0,15,30,45,
		width = dup.size().width();
		height = dup.size().height();
		sprintf(name,"exp/ROT_%d.jpg",i*ROT_DELTA);
		dup.write(name);
		/* For development purpose take small sample */
		Quantum *pixel_cache = dup.getPixels(0,0,width,height);
		hash = (int *)malloc(bits * bits * sizeof(int));
		blockhash_int(bits, pixel_cache, width, height, hash);
		free(hash);
	}
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

void Blockhash::translate_blocks_to_bits(Quantum *blocks, int nblocks, int pixels_per_block, int *hash)
{
    Quantum half_block_value;
    int bandsize, i, j, v;
    float m;

    half_block_value = pixels_per_block * CDEPTH * PIXEL_SIZE / 2; // For current configuration its 65536
    bandsize = nblocks / N_BANDS;

    for (i = 0; i < N_BANDS; i++) {
        m = median(&blocks[i * bandsize], bandsize);
        for (j = i * bandsize; j < (i + 1) * bandsize; j++) {
            v = blocks[j];
            hash[j] = v > m || (abs(v - m) < 1 && m > half_block_value);
        }
    }
}

char* Blockhash::bits_to_hexhash(int *bits, int nbits)
{
    int    i, j, b;
    int    len;
    int    tmp;
    char  *hex;
    char  *stmp;

    len = nbits / 4;

    hex = (char *)malloc(len + 1);
    stmp = (char *)malloc(2);
    hex[len] = '\0';

    for (i = 0; i < len; i++) {
        tmp = 0;
        for (j = 0; j < 4; j++) {
            b = i * 4 + j;
            tmp = tmp | (bits[b] << 3 >> j);
        }

        sprintf(stmp, "%1x", tmp);
        hex[i] = stmp[0];
        cout<<hex[i];
    }

    free(stmp);
    return hex;
}
void Blockhash::blockhash_int(int bits, Quantum *data, int width, int height, int *hash)
{
    int    i, x, y, ix, iy,j;
    int    ii;
    int    block_width;
    int    block_height;
    Quantum  *blocks;
    Quantum  value;

    block_width = width / bits;
    block_height = height / bits;
    //cout<<"[DEBUG] Height: "<<height<<", Width: "<<width<<endl;
	//cout<<"[DEBUG] Block Height: "<<block_height<<", Block Width: "<<block_width<<endl;

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
                    ii = ((y * block_height + iy) * width + (x * block_width + ix)) * PIXEL_SIZE;
					value += data[ii] + data[ii+1] + data[ii+2]; 
                }
            }
            blocks[y * bits + x] = value;
        }
    }
    translate_blocks_to_bits(blocks,bits*bits,block_height*block_width,hash);
    bits_to_hexhash(hash,bits*bits);
    cout<<endl;
}
