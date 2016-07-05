#ifndef GPG_H
#define GPG_H

#include <cstdbool>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

#include "const.h"
#include "log.h"
using namespace std;

class Gpg{
	private:
        string gen_cmd; // Sign command
        string verify_cmd; // Verification command
        bool error; // Error flag
        string err_msg; // Error message
        bool set_err(bool error,string err_msg); // Setter for error
        int exec_cmd(string& cmd,string& pattern); //Execute command using terminal
        bool exists(const string& name); //

	public:
        Gpg(); // Constructor
        bool verify(const string& fin,const string& fout); // Verify signature
        bool generate(const string& fin,const string& fout); // Sign hash file
        string get_err(); // Getter for error message
	};
#endif

