
#include <blockhash.h>

/*
 * Float compariosn for sorting
 */
static int cmpfloat(const void *pa, const void *pb)
{
    float a = *(const float *) pa;
    float b = *(const float *) pb;
    return (a < b) ? -1 : (a > b);
}

/*
 * Public constructor
 */
Blockhash::Blockhash():PIXEL_SIZE(CPIXEL_SIZE), ROT_DELTA(CROT_DELTA), OUT_PATH(COUT_PATH), HASH_FILE(CHASH_FILE), NUM_ROT(CNUM_ROT)
{
    error = false;
    err_msg	= "";
    img_path = "";
    num_hash_bits = 256;
    result = 1;
}

/*
 * Setter for error message
 */
bool Blockhash::set_err(bool error,string err_msg="")
{
    this->error = error;
    this->err_msg = err_msg;
    if(error)
        log_E(err_msg);
    return true;
}

/*
 * Getter for error message
 */
string Blockhash::get_err()
{
    return err_msg;
}

/*
 * Setter for progres
 */
int Blockhash::set_progress(int progress)
{
    this->progress = progress;
    emit progressValueChanged(this->progress);
    return 0;
}

/*
 * Getter for progress
 */

int Blockhash::get_progress()
{
    return progress;
}

/*
 *  Returns result of hash computation
 */

int Blockhash::get_result()
{
    return result;
}

/*
 *  Compute meadian of data
 */
float Blockhash::median(Quantum *data, int n)
{
    Quantum *sorted;
    float result;

    log_D("Computing Median");

    /* Sorting numbers in temporary array to find median */
    sorted = new Quantum[n];
    memcpy(sorted, data, n * sizeof(Quantum));
    qsort(sorted, n, sizeof(Quantum), cmpfloat);


    /* If number of elements are even then take average of two
     * middle numbers
     */

    if (n % 2 == 0) {
        result = (float) (sorted[n / 2] + sorted[n / 2 + 1]) / 2;
    } else {
        result = (float) sorted[n / 2];
    }

    log_D("Median is "<<result);
    delete sorted;
    return result;
}

int Blockhash::init_compute_hash(string const &fn, int bits)
{
    img_path = fn;
    num_hash_bits = bits;
    return 0;
}

void Blockhash::compute_hash()
{
    if(img_path!=""){
        result = compute_hash(img_path,num_hash_bits);
    }
    else{
        result = 1;
    }
}

/*
 * Process image and compute its hash
 */
int Blockhash::compute_hash(string const &fn, int bits)
{
    int i;
    int progress = 0;
    result = 1;
    size_t width, height;
    int *hash;
    Image image; //Original image
    Image dup; //Duplicate image
    image.read(fn.c_str()); //Read image
    image.modifyImage(); // Set modification flag to sync pixels after modification
    image.type(TrueColorType); //Set color to RGB format
	fstream f;
    f.open(HASH_FILE, fstream::out | fstream::trunc); // Create tem hash file
	f.close();
    set_err(false); //Reset error flag
    emit progressRangeChanged(0,NUM_ROT);
    set_progress(progress); //Set current porgress
    for(i=0;i<NUM_ROT;i++){
        /* Rotate image by multiples ROT_DELTA degree */
        dup = image;
		dup.modifyImage();
        dup.rotate(i*ROT_DELTA); //Rotation with 0,15,30,45

        /* Get image width and height */
        width = dup.size().width();
		height = dup.size().height();

        /* Get pixels value */
		Quantum *pixel_cache = dup.getPixels(0,0,width,height);

        /* Compute hash */
        hash = new int[bits*bits];
		blockhash_int(bits, pixel_cache, width, height, hash);

        /* Convert bit hash to hexadecimal */
        if(bits_to_hexhash(hash,bits*bits)!=0){
            delete hash;
            result = 1;
            emit finished();
            return result;
        }
        delete hash;
        progress += 1;
        set_progress(progress);
	}
    result = 0;
    emit finished();
    return result;
}

/*
 * Print pixel information
 */
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

/*
 * Convert each block to 1 or 0 depending upon meadin of band
 */
int Blockhash::translate_blocks_to_bits(Quantum *blocks, int nblocks, int pixels_per_block, int *hash)
{
    Quantum half_block_value;
    Quantum v,m;
    int bandsize, i, j;
    /* Compute half block value
     * Half Block Value: Half of maximum sum possible of block
     */
    half_block_value = pixels_per_block * CDEPTH * PIXEL_SIZE / 2; // For current configuration its 65536
    bandsize = nblocks / N_BANDS;

    /* Compute hash for bands instead of whole image */
    for (i = 0; i < N_BANDS; i++) {
        /* Compute median of band */
        m = median(&blocks[i * bandsize], bandsize);

        /*
         * Hash value is 1: if (Block value > Median) or ((Diff between median and block value
         * is less than 1) and (median is greater than block value))
         */
        for (j = i * bandsize; j < (i + 1) * bandsize; j++) {
            v = blocks[j];
            hash[j] = v > m || (abs(v - m) < 1 && m > half_block_value);
        }
    }
    return 0;
}

/*
 * Convert bits to hexadecimal
 */
int Blockhash::bits_to_hexhash(int *bits, int nbits)
{
    int    i, j, b;
    int    len;
    int    tmp;
    char  *hex;
    char  *stmp;
    set_err(false);
    /* Create temp directory to write intermediate hash files */
    mkdir(CDATA_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	ofstream myfile;

    /* Open file in write mode to write hash into file */
    myfile.open (HASH_FILE, ios::out | ios::app);
    if(!myfile.is_open()){
        set_err(true,"Unable to open file for writing hash");
        return 1;
    }
    len = nbits / 4;

    hex = new char[len+1];
    stmp = new char[2];
    hex[len] = '\0';

    /* Convert bits to hexadecimal */
    for (i = 0; i < len; i++) {
        tmp = 0;
        for (j = 0; j < 4; j++) {
            b = i * 4 + j;
            tmp = tmp | (bits[b] << 3 >> j);
        }

        /* Write hash to file */
        sprintf(stmp, "%1x", tmp);
        hex[i] = stmp[0];
        myfile<<hex[i];
    }
	myfile<<endl;
	myfile.close();

    delete stmp;
    delete hex;
    return 0;
}

/*
 * Divide image into blocks and compute its sum
 */
int Blockhash::blockhash_int(int bits, Quantum *data, int width, int height, int *hash)
{
    int    x, y, ix, iy;
    int    ii;
    int    block_width;
    int    block_height;
    int    rblock_height, rblock_width;
    Quantum  *blocks;
    Quantum  value;

    /* Height and width of each block */
    block_width = width / bits;
    rblock_width = width % bits;
    block_height = height / bits;
    rblock_height = height % bits;

    cout << bits << "X" << bits << ",  ";
    cout << rblock_height << "X" << rblock_width << ",  ";
    cout << block_height << "X" << block_width << endl;

    /* This will also initialize allocated memory to 0 */
    blocks = new Quantum[bits*bits]();

    for (y = 0; y < bits; y++) {
        for (x = 0; x < bits; x++) {
            value = 0;

            /* Process each block to compute its value*/
            for (iy = 0; iy < block_height; iy++) {
                for (ix = 0; ix < block_width; ix++) {
                    ii = ((y * block_height + iy) * width + (x * block_width + ix)) * PIXEL_SIZE;
					value += data[ii] + data[ii+1] + data[ii+2];
                }
            }
            /* Note down value of block */
            blocks[y * bits + x] = value;
        }
    }
    return translate_blocks_to_bits(blocks,bits*bits,block_height*block_width,hash);
}

/*
 * Compare two hashes
 */
float Blockhash::compare_hash(string const &hash_file1,string const &hash_file2)
{
	string line,line1;
	float curr_per=0.0f,perc=0.0f;
	int count;
	ifstream hashfile(hash_file1.c_str());
	ifstream hash(hash_file2.c_str());
    set_err(false);

    /* Read Hash file */
	if(!hashfile.is_open()){
        set_err(true,"Unable to open file: "+hash_file1);
		return -1;
		}

	if(!hash.is_open()){
        set_err(true,"Unable to open file: "+hash_file2);
		return -1;
		}

    /* Read line from hash size and compare bitwise */
	while(getline(hashfile,line)){
		while(getline(hash,line1)){
            /* If hash length is of different size then give error */
            if(line.length()!=line1.length()){
                set_err(true,"Hash sizes are different, cannot compare");
                return -1;
            }
            count=0;

            /* If complete line matches then 100% match */
            if(line.compare(line1)==0){
                perc=100;
            }
            else{
                /* Compare bit by bit */
                for(unsigned int i=0;i<line1.length();i++){
                    if(line[i]==line1[i])
                    count++;
                }

                /* Return max percentage */
                curr_per=((float)count/line1.length()) *100;
                if(curr_per > perc)
                    perc=curr_per;
                }
            /* If both hashes matched, avoid further computing */
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
