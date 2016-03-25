#include <iostream>
#include <cstring>
#include <Magick++.h>
#include "blockhash.h"

using namespace std;
using namespace Magick;

void help() {
    cout<<"Usage: blockhash [-h|--help] filenames...\n"
           "\n"
           "Optional arguments:\n"
           "-h, --help            Show this help message and exit\n";
}

int main(int argc, char **argv)
{
	InitializeMagick(*argv);
	Blockhash bh;	

    if (argc < 2) {
		help();
        return 0;
    }
    
    if(argc==2){
		if(!strcmp(argv[1],"--help") || !strcmp(argv[1],"-h")){
			help();
			return 0;
			}
		}
	bh.process_image(argv[1],16,1,1);
    return 0;
}
