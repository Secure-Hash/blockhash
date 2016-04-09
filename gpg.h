#ifndef _GPG_H
#define _GPG_H

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
		string gen_cmd;
		string verify_cmd;
		bool error;
		string err_msg;
		bool set_err(bool error,string err_msg);
		int exec_cmd(string& cmd,string& pattern);
		bool exists(const string& name);

	public:
		Gpg();
		bool verify(const string& fin,const string& fout);
		bool generate(const string& fin,const string& fout);
	};
#endif

