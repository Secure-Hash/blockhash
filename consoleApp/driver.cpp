#include "gpg.h"

int main(){
	if(Gpg::generate("hello.txt","hello.txt.asc")){
		cout<<"Successfully generated signature"<<endl;
		}
	if(Gpg::verify("hello.txt.asc","./.data/hello.txt")){
		cout<<"Signature verified"<<endl;
		}
	return 0;
	}
