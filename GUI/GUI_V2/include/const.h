#ifndef CONST_H
#define CONST_H

#define CPIXEL_SIZE     3
#define CROT_DELTA 		5
#define CNUM_ROT        19
#define CDEPTH          65536
#define COUT_PATH       ".data/"
#define CHASH_FILE		".data/hash.txt"
#define CDATA_DIR       ".data/"
#define N_BANDS 4

#ifndef DEBUG
    #define DEV_NULL    " > /dev/null 2>&1"
#else
    #define DEV_NULL    ""
#endif

#endif
