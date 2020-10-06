
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <string.h>
#include <stack>
#include <algorithm>

using namespace std;

struct Register{
    char name[30];
    char user[30];
    char mail[41];
    char pass[12];

    Register(){};

    Register(string nam, string usr, string ml, string pss){
        nam = nam.substr(0,29);
        usr = usr.substr(0,29);
        ml = ml.substr(0,40); 
        pss = pss.substr(0,11);      

        strcpy(name, (nam + string(5 - nam.length() ,' ')).c_str());
        
        strcpy(user, (usr + string(5 - usr.length() ,' ')).c_str());

        strcpy(mail, (ml + string(20 - ml.length() ,' ')).c_str());
        strcpy(pass, (pss + string(15 - pss.length() ,' ')).c_str());
    }
};

int main(){
    return 0;
}