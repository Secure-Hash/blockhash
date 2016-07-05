#include <iostream>
#include <cstring>
#include <Magick++.h>
#include "blockhash.h"
#include "gpg.h"

using namespace std;
using namespace Magick;

void help() {
    cout<<"Usage: blockhash [-h|--help] imagename output\n"
           "\n"
           "Optional arguments:\n"
           "-h, --help            Show this help message and exit\n";
}

int generate_hash(string const &imgpath, string &outpath)
{
	Blockhash bh;
	Gpg gpg;
	bool result;
	bh.process_image(imgpath,16,1,1);
	result = gpg.generate(CHASH_FILE,outpath + ".asc");
	if(!result){
		log_E("Bad signature");
		return 1;
	}
	return 0;
}

int verify_hash(string const &fin1,string const &fin2)
{
	Gpg gpg;
	Blockhash blockhash;
	string hash_file1,hash_file2;
	float result;
	hash_file1 = ".data/hash_file1";
	hash_file2 = ".data/hash_file2";
	if(!gpg.verify(fin1,hash_file1)){
		log_E("Bad signature");
		return -1;
		}
	if(!gpg.verify(fin2,hash_file2)){
		log_E("Bad signature");
		return -1;
		}
	result = blockhash.compare_hash(hash_file1,hash_file2);
	log_I("Similarity: "<<result);
	return 0;
}

int main(int argc, char **argv)
{
	InitializeMagick(*argv);
	Blockhash bh;
	bool result;
	string imgpath,outpath;
    if (argc < 4) {
		help();
        return 0;
    }

    if(argc==4){
		if(!strcmp(argv[1],"--help") || !strcmp(argv[1],"-h")){
			help();
			return 0;
			}
		}
	if(!strcmp(argv[3],"--verify")){
		string hash_file1,hash_file2;
		hash_file1 = argv[1];
		hash_file2 = argv[2];
		return verify_hash(hash_file1,hash_file2);
		}
	if(!strcmp(argv[3],"--generate")){
		imgpath = argv[1];
		outpath = argv[2];
		return generate_hash(imgpath,outpath);
		}
    return 0;
}