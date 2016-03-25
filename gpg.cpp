#include "gpg.h"

/** gpg --sign --armor --batch --yes --output outname.asc input */
string Gpg::gen_cmd		= "gpg --sign --armor --batch --yes --output ";

/**gpg --decrypt --batch --yes --output outname signed.asc*/
string Gpg::verify_cmd 	= "gpg --decrypt --batch --yes --output "; 

bool Gpg::error 		= false;
string Gpg::err_msg		= "";

bool Gpg::set_err(bool error,string err_msg="")
{
		Gpg::error = error;
		Gpg::err_msg = err_msg;
		if(error)
			log_E(err_msg);
}

bool Gpg::verify(const string& fin,const string& fout)
{
	set_err(false);
	log_D("Verification Signature, Input: "+fin+", Output:"+fout);
	remove(COUT_PATH); //No need to handle removal of directory
	mkdir(COUT_PATH,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); //Assume directory created successfully
	string command = verify_cmd + fout + " " + fin + DEV_NULL;
	if(!exists(fin)){
		set_err(true,"Unable to open signature file");
		log_D("Unable to open "+fin);
		return false;
		}
	if(system(command.c_str())!=0){
		set_err(true,"Invalid signature");
		return false;
		}
	if(!exists(fout)){
		set_err(true,"Unable to decrypt hash");
		log_D("Unable to open "+fout);
		return false;
		}
	return true;
}

bool Gpg::generate(const string& fin,const string& fout)
{
	int status;
	set_err(false);
	string command = gen_cmd + fout + " " + fin + DEV_NULL;
	log_D("Generating Signature, Input: "+fin+", Output:"+fout);
	log_D("[CMD] "+command);
	if(!exists(fin)){
		set_err(true,"Unable to open hash file");
		log_D("Unable to open "+fin);
		return false;
		}
	status=system(command.c_str());
	if(status!=0){
		log_D("Exit Code from hash generation "+status);
		return false;
		}
	return true;
}

inline bool Gpg::exists(const string& name) {
    if (FILE *file = fopen(name.c_str(), "rb")) {
        fclose(file);
        return true;
    } else {
        return false;
    } 
}
