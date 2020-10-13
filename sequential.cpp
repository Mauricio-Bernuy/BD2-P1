
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
    long posit = -1;
    file_sel file = d;
    pos(){}
    pos(int p, file_sel f):posit(p),file(f){}
};

struct s_Register{
    char name[30];
    char user[30];
    char mail[41];
    char pass[12];
    
    long pos = -1;
    file_sel file = d;
    bool deleted = false;

    s_Register(){};
    s_Register(string nam, string usr, string ml, string pss){
        if (nam.size() >= 30) nam = nam.substr(0,30);
        if (usr.size() >= 30) usr = usr.substr(0,30);
        if (ml.size() >= 41) ml = ml.substr(0,41);
        if (pss.size() >= 12) pss = pss.substr(0,12);
        strcpy(name, nam.c_str());
        strcpy(user, usr.c_str());
        strcpy(mail, ml.c_str());
        strcpy(pass, pss.c_str());
    }
};

inline vector<s_Register> registers;

inline bool s_reg_nom_comp(s_Register a, s_Register b){
    return string(a.name) < string(b.name);
}

inline streampos s_fileSize(string filename){
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
    int mem_access_counter_AUX, mem_access_counter_DATA = 0;
    SequentialFile(){};
    SequentialFile(string n, string csv = ""){
       construct(n,csv);
    };

    void construct(string n, string csv = ""){
        mem_access_counter_AUX = 0;
        mem_access_counter_DATA = 0;
        Name = n;
        if ((!csv.empty()) && s_fileSize(n) == 0) {
            load_data(csv);
            insertAll(registers);
            registers.clear();
        }
    }

    inline void load_data(string csv = "Usuario.csv"){
        std::ifstream Usuario(csv);
        
        string current;	
        while(getline(Usuario, current)){ {mem_access_counter_DATA++;}
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

            s_Register temp(name, user, mail, pass);
            registers.push_back(temp);
            
        }
    }

    vector<s_Register> load(){
        vector<s_Register> result;
        fstream if_datos(Name, ios::in | ios::binary);
        fstream if_aux("auxil.dat", ios::in | ios::binary);
        s_Register tmp;
        int next_pos = 0;
        file_sel next_coord;
        if (s_fileSize(Name) <= 0){
            next_coord = a;
            if (s_fileSize("auxil.dat") <= 0)
                return result;
        }
        else
            next_coord = d;

        while(next_pos != -1){
            if (next_coord == d){
                if_datos.seekg(next_pos, ios::beg);
                if_datos.read((char*) &tmp, sizeof(tmp)); {mem_access_counter_DATA++;}

            } else{
                if_aux.seekg(next_pos, ios::beg);
                if_aux.read((char*) &tmp, sizeof(tmp)); {mem_access_counter_AUX++;}
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

    void insertAll(vector<s_Register> registros){
        sort(registros.begin(),registros.end(),s_reg_nom_comp);

        for (auto i = registros.begin(); i != registros.end(); ++i){
            add(*i,false);
        }
    }

    pos getprev(s_Register record){
        ifstream if_datos(Name, ios::in | ios::binary);
        ifstream if_aux("auxil.dat", ios::in | ios::binary);

        s_Register tmp;
        int nx_ptr = 0;
        file_sel nx_file;
        pos prev;
        
        if (s_fileSize(Name) <= 0){
            nx_file = a;
            if (s_fileSize("auxil.dat") <= 0)
                return prev;
        }
        else
            nx_file = d;

        while(nx_ptr >= 0){
            if (nx_file == d){
                if_datos.seekg(nx_ptr, ios::beg);
                if_datos.read((char*) &tmp, sizeof(tmp));{mem_access_counter_DATA++;}

            } else{
                //if_aux.open("auxil.dat", ios::in | ios::binary);
                if_aux.seekg(nx_ptr, ios::beg);
                if_aux.read((char*) &tmp, sizeof(tmp));{mem_access_counter_AUX++;}
            }
            
            if (!(if_datos.good() || if_aux.good()) ) 
                break;

            //if (s_reg_nom_comp(tmp, record)){
            bool a = record.name < tmp.name;
            bool b = string(record.name) < string(tmp.name);
            if (!s_reg_nom_comp(record,tmp)){
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

    pos getnext(s_Register record){
        ifstream if_datos(Name, ios::in | ios::binary);
        ifstream if_aux("auxil.dat", ios::in | ios::binary);

        s_Register tmp;
        int nx_ptr = 0;
        file_sel nx_file;
        pos next;
        if (s_fileSize(Name) <= 0){
            nx_file = a;
            if (s_fileSize("auxil.dat") <= 0)
                return next;
        }
        else
            nx_file = d;

        while(nx_ptr >= 0){
            if (nx_file == d){
                if_datos.seekg(nx_ptr, ios::beg);
                if_datos.read((char*) &tmp, sizeof(tmp));{mem_access_counter_DATA++;}

            } else{
                if_aux.seekg(nx_ptr, ios::beg);
                if_aux.read((char*) &tmp, sizeof(tmp));{mem_access_counter_AUX++;}
            }
            
            if (!(if_datos.good() || if_aux.good())) 
                break;

            if (!s_reg_nom_comp(tmp, record)){
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

    s_Register search(string key){
        mem_access_counter_DATA = 0;
        mem_access_counter_AUX = 0;

        s_Register result;

        int pos = searchpos(key);

        if (pos != -1){
            ifstream if_datos(Name, ios::in | ios::binary);
            if_datos.seekg((pos * sizeof(result)), ios::beg);
            if_datos.read((char*) &result, sizeof(result));
            {mem_access_counter_DATA++;}
            if_datos.close();
        } 

        return result;
    }

    int searchpos(string key, bool offloadaux = true){
        key = key.substr(0,29);
        key = key + string(30 - key.length() ,' ');

        s_Register tmp;

        fstream ofs;
        ofs.open("auxil.dat", ios::out | ios::app | ios::binary);
        ofs.seekg(0, ios::end);

        if (ofs.tellg() != 0 && offloadaux){ // merge if aux is not empty
            merge();
        }
        
        ofs.close();

        int l = 0;
        int u = s_fileSize(Name)/sizeof(tmp)-1;

        while (u >= l){
            int m = (u + l)/2;
            ifstream if_datos(Name, ios::in | ios::binary);
            if_datos.seekg((m * sizeof(tmp)), ios::beg);
            if_datos.read((char*) &tmp, sizeof(tmp));{mem_access_counter_DATA++;}

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
        s_Register tmp;
        auto x = load();
        sort(x.begin(), x.end(), s_reg_nom_comp);
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
            ofs.write((char*) &*i, sizeof(*i)); {mem_access_counter_DATA++;}
        }
        ofs.close();
    }

    void add(s_Register record, bool count = true){
        if (count){
            mem_access_counter_DATA = 0;
            mem_access_counter_AUX = 0;
        }
        s_Register tmp;
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
        ofs.write((char*) &record, sizeof(record)); {mem_access_counter_DATA++;}
        ofs.close();

        if(prev.posit != -1){
            fstream edit;
            if (prev.file == d)
                edit.open(Name, ios::in | ios::out | ios::binary);
            else 
                edit.open("auxil.dat", ios::in | ios::out | ios::binary);
            
            int i = 0;
            edit.seekg(prev.posit, ios::beg);
            edit.read((char*) &tmp, sizeof(tmp)); {i++;}
            tmp.pos = position;
            tmp.file = a;
            edit.seekp(prev.posit, ios::beg);
            edit.write((char*) &tmp, sizeof(tmp)); {i++;}
            edit.close();

            if (prev.file == d)
                {mem_access_counter_DATA += i;}
            else 
                {mem_access_counter_AUX += i;}
        }
    }   

    bool delet(string key){
        mem_access_counter_DATA = 0;
        mem_access_counter_AUX = 0;
        
        s_Register tmp, prevreg;

        int position = searchpos(key);
        if (position == -1) return false;

        ifstream if_datos(Name, ios::in | ios::binary);
        if_datos.seekg((position * sizeof(tmp)), ios::beg);
        if_datos.read((char*) &tmp, sizeof(tmp)); {mem_access_counter_DATA++;}
        if_datos.close();

        pos prev = getprev(tmp);
        pos next = getnext(tmp);

        tmp.deleted = true;

        fstream edit;

        edit.open(Name, ios::in | ios::out | ios::binary);
        edit.seekp((position* sizeof(tmp)), ios::beg);
        edit.write((char*) &tmp, sizeof(tmp)); {mem_access_counter_DATA++;}
        edit.close();

        if (prev.file == d)
            edit.open(Name, ios::in | ios::out | ios::binary);
        else 
            edit.open("auxil.dat", ios::in | ios::out | ios::binary);
        
        int i = 0;
        edit.seekg(prev.posit, ios::beg);
        edit.read((char*) &prevreg, sizeof(prevreg)); {i++;}
        prevreg.pos = next.posit;
        prevreg.file = next.file;
        edit.seekp(prev.posit, ios::beg);
        edit.write((char*) &prevreg, sizeof(prevreg));{i++;}

        edit.close();

        if (prev.file == d)
            {mem_access_counter_DATA += i;}
        else 
            {mem_access_counter_AUX += i;}
        return true;
    }    
};

int main(){
    SequentialFile asj("data.dat");
    asj.insertAll(registers);
    asj.add(s_Register("aaron","lol","lol","lol"));
    auto s = asj.search("Aeris Oliver");
    auto b = asj.delet("Rocco Wright");
    s = asj.search("Rocco Wright");
    b = asj.delet("Anthony Briggs");
    s = asj.search("Aeris Oliver");
    s = asj.search("Gina Connor");
    cout<<"done!\n";
}
