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
		static string gen_cmd;
		static string verify_cmd;
		static bool error;
		static string err_msg;
		static bool set_err(bool error,string err_msg);
		static int exec_cmd(string& cmd,string& pattern);
	public:
		static bool verify(const string& fin,const string& fout);
		static bool generate(const string& fin,const string& fout);
		static bool exists(const string& name);
	};
#endif

