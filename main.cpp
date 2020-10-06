
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <string.h>
#include <stack>
#include <algorithm>
#include <sstream>

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

        strcpy(name, (nam + string(30 - nam.length() ,' ')).c_str());
        strcpy(user, (usr + string(30 - usr.length() ,' ')).c_str());
        strcpy(mail, (ml + string(41 - ml.length() ,' ')).c_str());
        strcpy(pass, (pss + string(12 - pss.length() ,' ')).c_str());
    }
};

vector<Register> registers;

void load_data(){
	std::ifstream Usuario("Usuario.csv");
	
	string current;	
	while(getline(Usuario, current)){
        string name, user, mail, pass; 
		stringstream ss(current);

		getline(ss, current, ',');
		name = current;
		getline(ss, current, ',');
        user = current;
        getline(ss, current, ',');
        mail = current;
        getline(ss, current, ',');
        pass = current;

        Register temp(name, user, mail, pass);
		registers.push_back(temp);
	}
}



int main(){
    load_data();
    cout<<"done!\n";
    return 0;
}