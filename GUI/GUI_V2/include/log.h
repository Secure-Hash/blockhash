#ifndef LOG_H
#define LOG_H
#include <iostream>

using namespace std;
#define log_I(MESSAGE) cout<<"[INFO] "<< MESSAGE << endl;
#define log_E(MESSAGE) cout<<"[ERROR] "<< MESSAGE <<endl;

#ifdef DEBUG 
    #define log_D(MESSAGE) cout<<"["<<__FILE__<<":"<<__LINE__<<"][DEBUG] " <<MESSAGE<<endl;
#else 
    #define log_D(MESSAGE)
#endif	

#endif
