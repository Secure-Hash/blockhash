
#include "blockhash.h"

static int cmpfloat(const void *pa, const void *pb)
{
    float a = *(const float *) pa;
    float b = *(const float *) pb;
    return (a < b) ? -1 : (a > b);
}

Blockhash::Blockhash():PIXEL_SIZE(CPIXEL_SIZE), ROT_DELTA(CROT_DELTA), OUT_PATH(COUT_PATH), HASH_FILE(CHASH_FILE)
{
}

Blockhash::~Blockhash()
{
}

float Blockhash::median(Quantum *data, int n)
{
    Quantum *sorted;
    float result;

    log_D("Computing Median");
    sorted = (Quantum *)malloc(n * sizeof(Quantum));
    memcpy(sorted, data, n * sizeof(Quantum));
    qsort(sorted, n, sizeof(Quantum), cmpfloat); //Assuming Quantum as float, make it installation independent

    if (n % 2 == 0) {
        result = (float) (sorted[n / 2] + sorted[n / 2 + 1]) / 2;
    } else {
        result = (float) sorted[n / 2];
    }
    log_D("Median is "<<result);
    free(sorted);
    return result;
}

int Blockhash::process_image(string const &fn, int bits, int quick, int debug)
{
    int i, j;
    size_t width, height;
    int *hash;
    char name[10];
    Image image;
    Image dup;
    image.read(fn.c_str());
    image.modifyImage();
    image.type(TrueColorType);

	fstream f;
	f.open(HASH_FILE, fstream::out | fstream::trunc);
	f.close();

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
		bits_to_hexhash(hash,bits*bits);
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

	ofstream myfile;
	myfile.open (HASH_FILE, ios::out | ios::app);

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
        myfile<<hex[i];
    }
	myfile<<endl;
	myfile.close();

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
}

float Blockhash::compare_hash(string const &hash_file1,string const &hash_file2)
{
	string line,line1;
	float curr_per=0.0f,perc=0.0f;
	int count;
	ifstream hashfile(hash_file1.c_str());
	ifstream hash(hash_file2.c_str());

	/* Read Hash file */
	if(!hashfile.is_open()){
		log_E("Unable to open file\n");
		return -1;
		}

	if(!hash.is_open()){
		//TODO Set Error for GUI
		log_E("Unable to open file\n");
		return -1;
		}

	while(getline(hashfile,line)){
		while(getline(hash,line1)){
			count=0;
			if(line.compare(line1)==0){
				perc=100;
			}
			else{
				for(int i=0;i<line1.length();i++){
					if(line[i]==line1[i])
					count++;
				}
				curr_per=((float)count/line1.length()) *100;
				if(curr_per > perc)
					perc=curr_per;
			}
			if(perc==100)
				break;
		}
		if(perc==100)
			break;
		hash.clear();
		hash.seekg(0, ios::beg);
	}
	hashfile.close();
	hash.close();
	return perc;
}