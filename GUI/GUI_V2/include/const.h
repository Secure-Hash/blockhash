#ifndef _CONST_H
#define _CONST_H

#define CPIXEL_SIZE     3
#define CROT_DELTA 		15
#define CDEPTH          65536
#define COUT_PATH       ".data/"
#define CHASH_FILE		".data/hash.txt"

#ifndef DEBUG
#define DEV_NULL       " > /dev/null 2>&1"
#else
#define DEV_NULL ""
#endif

#define N_BANDS 4
#endif
