#include <iostream>
#include<string.h>
#include<map>
#include<fstream>
#include<cstdio>
#include<vector>
#include<bits/stdc++.h> 
#include<algorithm>
#include<tuple>
#include<thread>

using namespace std;

#define INDEX_SIZE 4
#define PAGE_SIZE 4
#define INDEX_LEVELS 1

mutex mtx;

struct Register{
  char name[30];
  char user[30];
  char mail[41];
  char pass[12];

  Register(const char nam[30],const char usr[30],const char ml[41],const char pss[12]){
      strcpy(name, nam);
      strcpy(user, usr);
      strcpy(mail, ml);
      strcpy(pass, pss);

      cout << "Defined name as " << name << endl;
      cout << "Defined user as " << user << endl;
      cout << "Defined mail as " << mail << endl;
      cout << "Definned pass as " << pass << endl;
  }
  Register(){
     fill(begin(name), end(name), 0); // clear
     fill(begin(user), end(user), 0); // clear
     fill(begin(mail), end(mail), 0); // clear
     fill(begin(pass), end(pass), 0); // clear
  };
  void print(){
    cout << "------------------------------"<<endl;
    cout << "Nombre: " << name << endl;
    cout << "Usuario: " << user << endl;
    cout << "Mail: " << mail << endl;
    cout << "Password: " << pass << endl;
    cout << "------------------------------"<<endl;
  };
};

struct Page{
  Register records[PAGE_SIZE];
  
  long next_bucket; // if filled, will point to next bucket 
  int first_empty; // pos of first empty in page, if PAGE_SIZE -> bucket full

  Page(){};
  Page(vector<Register> in){   
    for (auto i = 0; i < in.size(); ++i)
      records[i] = in[i];

    first_empty = in.size();
    next_bucket = -1;
  };

  string getkey(){
    return records[0].name;
  }

};

struct Index{
    long address;
    char key[30];

    Index(){};
    Index(long add, const char k[30]):address(add){
      strcpy(key, k);
    };
};

struct PageLocation{
  Register regist;
  long address = -1;
  int index = -1;

  bool exists = true;
  
  PageLocation(){};
  PageLocation(Register reg, long add, int ind, bool ex = true):regist(reg), address(add), index(ind), exists(ex){};
};

struct Query{
  short operation; // Insert = -2, delete = 1; select = 2;
  string key;
  Register reg;
};


struct IndexLvl{
  Index indexes[INDEX_SIZE];
};

inline istream& operator>> (istream& stream, Register & record){
  stream.read((char*) &record, sizeof(record));
  return stream;
}

inline ostream& operator<< (ostream& stream, Register & record){
  stream.write((char*) &record, sizeof(record));
  return stream;
}

inline istream& operator>> (istream& stream, Index & record){
  stream.read((char*) &record, sizeof(record));
  return stream;
}

inline ostream& operator<< (ostream& stream, Index & record){
  stream.write((char*) &record, sizeof(record));
  return stream;
}

inline ostream& operator<< (ostream& stream, Page & page){
  stream.write((char*) &page, sizeof(page));
  return stream;
}

inline istream& operator>> (istream& stream, Page & page){
  stream.read((char*) &page, sizeof(page));
  return stream;
}

inline void sort(Register *arr, int low, int high, int mid){
  int i, j, k;
  Register c[high+1];
	i = low;
	k = low;
	j = mid + 1;
	while (i <= mid && j <= high) {
			if (arr[i].name < arr[j].name) {
					c[k] = arr[i];
					k++;
					i++;
			}
			else  {
					c[k] = arr[j];
					k++;
					j++;
			}
	}
	while (i <= mid) {
			c[k] = arr[i];
			k++;
			i++;
	}
	while (j <= high) {
			c[k] = arr[j];
			k++;
			j++;
	}
	for (i = low; i < k; i++)  {
			arr[i] = c[i];
	}
}

inline void MergeSort(Register* vec, int low, int high){
  if(low < high){
    int temp = (low + high)/2;
    MergeSort(vec, low, temp);
    MergeSort(vec, temp + 1, high);
    sort(vec, low, high, temp);
  }
}

inline bool reg_nom_comp(Register a, Register b){
    return string(a.name) < string(b.name);
}

inline streampos fileSize(string filename){
    streampos fsize = 0;
    ifstream file (filename, ios::binary);

    fsize = file.tellg();
    file.seekg (0, ios::end);
    fsize = file.tellg() - fsize;
    file.close();

    return fsize;
}

class ISAM{
  private:
    string fileName;
    string indexName; // TO DO -> array of levels 
    vector<Index> index = {}; // diccionario en memoria principal
    bool done = false;

  public:
    string getfileName() {return fileName;};
    string getindexName() {return indexName;};

    void loadIndex(){ // TO DO -> array of levels 
      Index idx;
      ifstream in_idx(indexName, ios::binary); if (!in_idx.is_open()) return;
      while(in_idx >> idx)
        index.push_back(idx);
      in_idx.close();
    }

    ISAM(){};

    ISAM(string _fileName, string csv = ""){
      construct(_fileName, csv);
    }

    void construct(string _fileName, string csv = ""){
      fileName = _fileName;
      indexName = fileName.substr(0, fileName.length()-4) + "_index" + to_string(1) + ".dat";
     
      if (fileSize(fileName) == 0 && !csv.empty()) csv2dat();
      if (fileSize(indexName) == 0) build_index();  
      loadIndex();
    }

    ~ISAM(){}

    void csv2dat(string csv = "Usuario.csv"){
      vector<Register> registers;

      // load and parse csv
      std::ifstream Usuario(csv);
      string current;	

      while (getline(Usuario, current)){
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

        Register temp(name.c_str(), user.c_str(), mail.c_str(), pass.c_str());
        registers.push_back(temp);
      }

      sort(registers.begin(),registers.end(),reg_nom_comp);
      
      // write pages to data file 
      ofstream out(fileName, ios::app | ios::binary);
      vector<Register> records;
      for (auto i = 0; i < registers.size(); ++i){
        records.push_back(registers[i]);
        if (records.size() == PAGE_SIZE || !(i < registers.size()-1)){
          Page pageout(records);
          out.write((char*) &pageout, sizeof(pageout));
          records.clear();
        }
      }
      out.close();
    }

    void build_index(int levels = INDEX_LEVELS){
      int level = 1;

      ifstream data(fileName, ios::binary);
      ofstream out(indexName, ios::app | ios::binary);

      Page tmp;
      int i = 0;

      while(data >> tmp){
        Index idx((i * sizeof(tmp)), tmp.getkey().c_str());
        out << idx;
        i++;
      }
    }

    PageLocation search(string key, short lock){
      cout << "In search with key" << key << "and lock " << lock<< endl;
      PageLocation empty, first_free;
      Register reg_empty;

      fstream datafile(fileName, ios::out | ios::in | ios::ate | ios::app | ios::binary);
      if(!datafile.is_open()) throw("Unable to open files");
      int temp = 0;
      int l = 0;
      int u = index.size()-1;

      cout << "flag1" << endl;
      if (key < index[0].key){   
        Page paged;  
        long address = 0;
        datafile.seekg(address);
        datafile >> paged;
        cout << "flag2" << endl;

        while (paged.next_bucket != -1){
          cout << "flag3" << endl;
          address = paged.next_bucket;
          datafile.seekg(address);
          datafile >> paged;
        }
        
        PageLocation less(reg_empty, address, -2, false);
        return less;
      }
      cout << "flaag" << endl;
      while (u >= l){
        temp = (l+u)/2;
        if (key < index[temp].key) u = temp - 1;
        else{
          if (key > index[temp].key){
            if (key < index[temp+1].key) break;
            l = temp + 1;
          }
          else break;
        }
      }
      if(done == false)
        if(lock == 1) mtx.lock();
      if (temp >= index.size()) 
        return empty;
      
      long address = index[temp].address;
      cout << address << endl;
      datafile.seekg(address);

      Page curr_page;
      datafile >> curr_page;
      auto iterator = curr_page;
      bool found_first = false;
      while (1){
        int i = 0;
        if (iterator.first_empty < INDEX_SIZE && !found_first){
          PageLocation result(reg_empty, address, iterator.first_empty, false);
          first_free = result;
          found_first = true;
        }

        for (auto it = 0; it < iterator.first_empty; ++it){
          if (iterator.records[it].name == key){
            PageLocation result(iterator.records[it], address, i);
            cout << key << " was found!" << endl;
            return result;
          }
          i++;
        }
        if (iterator.next_bucket != -1){
          address = iterator.next_bucket;
          datafile.seekg(iterator.next_bucket);
          datafile >> iterator;
          
        } else {
          cout << "Unable to locate key" << endl;
          PageLocation result(reg_empty, address, iterator.first_empty, false); // should return first available bucket for insert
          if (found_first) return first_free;
          else return result;
        }
      }
    }

    bool erase(string key, short lock){
      cout << "In delete with key " << key<< "and lock " << lock << endl;
      PageLocation p;
      if(done == false)
        if(lock == 2) mtx.lock();
      fstream datafile(fileName, ios::out | ios::in | ios::ate | ios::binary);
      if(!datafile.is_open()) throw("Unable to open files");

      p = search(key, lock);
      if(!p.exists){ mtx.unlock(); return false; }
      if (p.address != -1 && p.index != -1){
          datafile.seekg(p.address);
          Page pag;
          datafile >> pag;
          pag.records[p.index] = pag.records[pag.first_empty -1];
          pag.first_empty--;
          MergeSort(pag.records, 0, pag.first_empty - 1);
          datafile.seekp(p.address);
          datafile << pag;
          datafile.close();
          mtx.unlock();
          return true;
          
      } else throw ("Unable to locate register");
      if(done == false){
        mtx.unlock();
        done = true;
      }
      return false;
    }

    bool insert(Register reg, short lock){
      cout << "In insert with reg" << endl;
      if(done == false)
        if(lock == 2) mtx.lock();
      fstream datafile(fileName, ios::out | ios::in | ios::ate | ios::binary);
      if(!datafile.is_open()) throw("Unable to open files");
      PageLocation p = search(reg.name, lock);
      if (p.exists) { mtx.unlock(); return false; }

      datafile.seekg(p.address);
      Page pag;
      datafile >> pag;

      if (p.index == -2){ //if smaller than first
        strcpy(index[0].key, reg.name);
        // rewrite to index
        fstream indexfile(indexName, ios::out | ios::in | ios::ate | ios::binary);
        indexfile.seekp(0);
        indexfile << index[0];
        indexfile.close();
        p.index = pag.first_empty;
      }

      if (p.index == 4){ // if needs to create overflow
        datafile.seekp(0, ios::end);
        pag.next_bucket = datafile.tellp();
        datafile.seekp(p.address);
        datafile << pag;

        p.address = pag.next_bucket;
        p.index = 0;
        Page new_page;
        new_page.first_empty = 0;
        new_page.next_bucket = -1;
        pag = new_page;
      }

      pag.records[pag.first_empty] = reg;
      pag.first_empty++;
      MergeSort(pag.records, 0, pag.first_empty - 1);
      datafile.seekp(p.address);
      datafile << pag;
      datafile.close();
      if(done == false){
        mtx.unlock();
        done = true;
      }
      return true;
    }
    
    void execute_query(Query query, short lock){
      PageLocation pg;
      switch(query.operation){
        case 2:
          //search
          pg = search(query.key, lock);
        case -2:
          //insert
          if (insert(query.reg, lock)) cout << "Inserted!" << endl;
          else cout << "Insertion invalid" << endl;
          break;
        case 1:
          //delete
          if (erase(query.key, lock)) cout << "Erased!" << endl;
          else cout << "Erase invalid" << endl;
          break;
        default:
          cout << "Invalid query" << endl;
          throw;
      }
    }

    void run(Query q1, Query q2){
      done = false;
      short lock = 0;
      short op1 = q1.operation;
      short op2 = q2.operation;
      short op_flag = op1 + op2;
      Query lowest = (op1 < op2) ? q1 : q2;
      Query biggest = (op1 > op2) ? q2 : q1;
      if(op1 == 3 || op2 == 3){ cout << "Query invalid" << endl; throw; }
      if(op_flag == 0){
        if(!strcmp(q1.key.c_str(), q2.key.c_str())) lock = 1;
        if(strcmp(lowest.key.c_str(), index[0].key) <= 0) lock = 2;
      }
      else{
        if(op_flag == 3) lock = 1;
        else{
          if(op_flag != -1) {
            if(!strcmp(q1.key.c_str(), q2.key.c_str())){
              thread qu2(&ISAM::execute_query, this, q1, lock);
              qu2.join();
              return;
            };
            if(strcmp(lowest.key.c_str(), index[0].key) <= 0) lock = 2;
          }
          else{
            lock = 2;
            thread qu1(&ISAM::execute_query, this, q1, 0);
            thread qu2(&ISAM::execute_query, this, q2, lock);
            qu1.join();
            qu2.join();
            return;
          }
        }
      }
      thread qu1(&ISAM::execute_query, this, lowest, 0);
      thread qu2(&ISAM::execute_query, this, biggest, lock);
      qu1.join();
      qu2.join();
      return;
    }
};

/*int main(){
  ISAM ourISAM("Registro de Usuarios.dat", "Usuario.csv");
 /* Query q1;
  Query q2;
  Query q3;
  Register reg1("Roger Wilson", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2");
  q1.operation = -2;
  q1.reg = reg1;

  q3.operation = 2;
  q3.key = "Roger Wilson";

  q2.operation = 1;
  q2.key = "Roger Wilson";

  ourISAM.run(q1, q2);*/
  auto result = ourISAM.search("Alexusis Fulton", 0);
  cout << "1" << endl;
  //ourISAM.erase("Kurt Nelson", 0);

  ourISAM.insert(Register("Roger Wilson", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2"), 0);
  cout << "2" << endl;
  ourISAM.insert(Register("Kurt Nelson", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2"), 0);
  cout << "3" << endl;
  ourISAM.insert(Register("Athena Lloyd", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2"), 0);
  cout << "4" << endl;
  ourISAM.insert(Register("Aaron Carter", "roger_wilson","roger_wilson@correo.com","WswASDw123Sd2"), 0);
  cout << "5" << endl;
  ourISAM.erase("Rocco Nelson", 0);
/*
  ifstream if_datafile(ourISAM.getfileName(), ios::in | ios::binary);
  ifstream if_indexfile(ourISAM.getindexName(), ios::in | ios::binary);
  Page pag;
  vector<Page> pag_res;
  Index ind;
  vector<Index> ind_res;

  while(if_datafile >> pag)
    pag_res.push_back(pag);
  
  while(if_indexfile >> ind)
    ind_res.push_back(ind);
  */
  cout<<"done!\n";
}
*/
