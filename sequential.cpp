#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <string.h>
#include <stack>
#include <algorithm>
#include <sstream>

using namespace std;

enum file_sel {a,d};

struct pos{
    int posit = -1;
    file_sel file = d;
    pos(){}
    pos(int p, file_sel f):posit(p),file(f){}
};

struct Register{
    char name[30];
    char user[30];
    char mail[41];
    char pass[12];

    int pos = -1;
    file_sel file = d;
    bool deleted = false;

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

bool reg_nom_comp(Register a, Register b){
    return string(a.name) < string(b.name);
}

streampos fileSize(string filename){
    streampos fsize = 0;
    ifstream file (filename, ios::binary);

    fsize = file.tellg();
    file.seekg (0, ios::end);
    fsize = file.tellg() - fsize;
    file.close();

    return fsize;
}

class SequentialFile{
private:
    string Name;
public:
    SequentialFile(string n):Name(n){};
    
    vector<Register> load(){
        vector<Register> result;
        fstream if_datos(Name, ios::in | ios::binary);
        fstream if_aux("auxil.dat", ios::in | ios::binary);
        Register tmp;
        int next_pos = 0;
        file_sel next_coord;
        if (fileSize(Name) <= 0){
            next_coord = a;
            if (fileSize("auxil.dat") <= 0)
                return result;
        }
        else
            next_coord = d;

        while(next_pos != -1){
            if (next_coord == d){
                if_datos.seekg(next_pos, ios::beg);
                if_datos.read((char*) &tmp, sizeof(tmp));

            } else{
                if_aux.seekg(next_pos, ios::beg);
                if_aux.read((char*) &tmp, sizeof(tmp));
            }
            
            if (!(if_datos.good() || if_aux.good())) 
                break;

            //if not deleted then
            result.push_back(tmp);   
            next_pos = tmp.pos;
            next_coord = tmp.file;
        } 
        if_datos.close();
        if_aux.close();
        return result;
    }

    void insertAll(vector<Register> registros){
        sort(registros.begin(),registros.end(),reg_nom_comp);

        for (auto i = registros.begin(); i != registros.end(); ++i){
            add(*i);
        }
    }

    pos getprev(Register record){
        ifstream if_datos(Name, ios::in | ios::binary);
        ifstream if_aux("auxil.dat", ios::in | ios::binary);

        Register tmp;
        int nx_ptr = 0;
        file_sel nx_file;
        pos prev;
        
        if (fileSize(Name) <= 0){
            nx_file = a;
            if (fileSize("auxil.dat") <= 0)
                return prev;
        }
        else
            nx_file = d;

        while(nx_ptr >= 0){
            if (nx_file == d){
                if_datos.seekg(nx_ptr, ios::beg);
                if_datos.read((char*) &tmp, sizeof(tmp));

            } else{
                //if_aux.open("auxil.dat", ios::in | ios::binary);
                if_aux.seekg(nx_ptr, ios::beg);
                if_aux.read((char*) &tmp, sizeof(tmp));
            }
            
            if (!(if_datos.good() || if_aux.good()) ) 
                break;

            if (reg_nom_comp(tmp, record)){
                prev.posit = nx_ptr;
                prev.file = nx_file;
            } else break;

            nx_ptr = tmp.pos;
            nx_file = tmp.file;
        } 
        if_datos.close();
        if_aux.close();

        return prev;
    }

    pos getnext(Register record){
        ifstream if_datos(Name, ios::in | ios::binary);
        ifstream if_aux("auxil.dat", ios::in | ios::binary);

        Register tmp;
        int nx_ptr = 0;
        file_sel nx_file;
        pos next;
        if (fileSize(Name) <= 0){
            nx_file = a;
            if (fileSize("auxil.dat") <= 0)
                return next;
        }
        else
            nx_file = d;

        while(nx_ptr >= 0){
            if (nx_file == d){
                if_datos.seekg(nx_ptr, ios::beg);
                if_datos.read((char*) &tmp, sizeof(tmp));

            } else{
                if_aux.seekg(nx_ptr, ios::beg);
                if_aux.read((char*) &tmp, sizeof(tmp));
            }
            
            if (!(if_datos.good() || if_aux.good())) 
                break;

            if (!reg_nom_comp(tmp, record)){
                next.posit = nx_ptr;
                next.file = nx_file;
                break;
            }

            nx_ptr = tmp.pos;
            nx_file = tmp.file;
        } 
        if_datos.close();
        if_aux.close();

        return next;
    }

    Register search(string key){
        Register result;

        int pos = searchpos(key);

        if (pos != -1){
            ifstream if_datos(Name, ios::in | ios::binary);
            if_datos.seekg((pos * sizeof(result)), ios::beg);
            if_datos.read((char*) &result, sizeof(result));
            if_datos.close();
        } 

        return result;
    }

    int searchpos(string key, bool offloadaux = true){
        key = key.substr(0,29);
        key = key + string(30 - key.length() ,' ');

        Register tmp;

        fstream ofs;
        ofs.open("auxil.dat", ios::out | ios::app | ios::binary);
        ofs.seekg(0, ios::end);

        if (ofs.tellg() != 0 && offloadaux){ // merge if aux is not empty
            merge();
        }
        
        ofs.close();

        int l = 0;
        int u = fileSize(Name)/sizeof(tmp)-1;

        while (u >= l){
            int m = (u + l)/2;

            ifstream if_datos(Name, ios::in | ios::binary);
            if_datos.seekg((m * sizeof(tmp)), ios::beg);
            if_datos.read((char*) &tmp, sizeof(tmp));
            if_datos.close();
            if (tmp.deleted == true) 
                u = m - 1;
            else{
                string check = string(tmp.name).substr(0,30);

                if (key == check) 
                    return m;

                if (key < check)
                    u = m - 1;
                else if (key > check)
                    l = m + 1;
            }                
        }
        return -1;
    }

    void merge(){
        Register tmp;
        auto x = load();
        sort(x.begin(), x.end(), reg_nom_comp);
        int next_ptr = 1;
        for (auto i = x.begin(); i != x.end(); ++i){
            i->pos = sizeof(tmp)*(next_ptr++);
            i->file = d;
        }
        x.back().pos = -1;
        x.back().file = d;
        
        fstream clr ("auxil.dat", ios::out | ios::trunc);
        clr.close();
        clr.open(Name, ios::out | ios::trunc);
        clr.close();

        fstream ofs;
        ofs.open(Name, ios::out | ios::app | ios::binary);
        for (auto i = x.begin(); i != x.end(); ++i){
            ofs.write((char*) &*i, sizeof(*i));
        }
        ofs.close();
    }

    void add(Register record){
        Register tmp;
        fstream ofs;
        
        ofs.open("auxil.dat", ios::out | ios::app | ios::binary);
        ofs.seekg(0, ios::end);

        long position = ofs.tellg(); // BYTES

        if (searchpos(record.name, false) != -1){
            cout<<"key already present\n";
            return;
        }

        if (position/sizeof(tmp) >= 5){
            merge();
            position = 0;
        }

        pos prev = getprev(record);
        pos next = getnext(record);
        record.pos = next.posit;
        record.file = next.file;

        ofs.seekp(0, ios::end);
        ofs.write((char*) &record, sizeof(record));
        ofs.close();

        if(prev.posit != -1){
            fstream edit;
            if (prev.file == d)
                edit.open(Name, ios::in | ios::out | ios::binary);
            else 
                edit.open("auxil.dat", ios::in | ios::out | ios::binary);
            
            edit.seekg(prev.posit, ios::beg);
            edit.read((char*) &tmp, sizeof(tmp));
            tmp.pos = position;
            tmp.file = a;
            edit.seekp(prev.posit, ios::beg);
            edit.write((char*) &tmp, sizeof(tmp));
            edit.close();
        }
    }   

    bool delet(string key){
        Register tmp, prevreg;

        int position = searchpos(key);
        if (position == -1) return false;

        ifstream if_datos(Name, ios::in | ios::binary);
        if_datos.seekg((position * sizeof(tmp)), ios::beg);
        if_datos.read((char*) &tmp, sizeof(tmp));
        if_datos.close();

        pos prev = getprev(tmp);
        pos next = getnext(tmp);

        tmp.deleted = true;

        fstream edit;

        edit.open(Name, ios::in | ios::out | ios::binary);
        edit.seekp((position* sizeof(tmp)), ios::beg);
        edit.write((char*) &tmp, sizeof(tmp));
        edit.close();

        if (prev.file == d)
            edit.open(Name, ios::in | ios::out | ios::binary);
        else 
            edit.open("auxil.dat", ios::in | ios::out | ios::binary);
        
        edit.seekg(prev.posit, ios::beg);
        edit.read((char*) &prevreg, sizeof(prevreg));
        prevreg.pos = next.posit;
        prevreg.file = next.file;
        edit.seekp(prev.posit, ios::beg);
        edit.write((char*) &prevreg, sizeof(prevreg));

        edit.close();

        return true;
    }    
};


int main(){
    /*Register test1("0001","telberto","industrial",2);
    Register test2("0002","alborto","economia",1);  
    Register test3("0003","elborarto","economia",1);        
    Register test4("0004","floborto","economia",1);    
    Register test5("0005","alimorto","economia",1);    
    Register test6("0006","belelmorto","economia",1); 
    vector<Register> lol;
    lol.push_back(test1);
    lol.push_back(test2);
    lol.push_back(test3);
    lol.push_back(test4);
    lol.push_back(test5);
    lol.push_back(test6);*/

    load_data();

    SequentialFile asj("data.dat");
    asj.insertAll(registers);
    auto s = asj.search("Aeris Oliver");
    auto b = asj.delet("Rocco Wright");
    s = asj.search("Rocco Wright");
    b = asj.delet("Anthony Briggs");
    s = asj.search("Aeris Oliver");
    s = asj.search("Gina Connor");
    cout<<"done!\n";
}
