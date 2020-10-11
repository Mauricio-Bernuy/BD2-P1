#include<iostream>
#include<string.h>
#include<map>
#include<fstream>
#include<cstdio>
#include<vector>
#include <bits/stdc++.h> 
#include <algorithm>
#include <tuple>

using namespace std;

#define INDEX_SIZE 4
#define PAGE_SIZE 4
#define INDEX_LEVELS 1

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
  Register(){};
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
  long first_empty; // pos of first empty in page, if PAGE_SIZE -> bucket full

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
  
  PageLocation(){};
  PageLocation(Register reg, long add, int ind):regist(reg), address(add), index(ind){};
};

struct IndexLvl{
  Index indexes[INDEX_SIZE];
};

istream& operator>> (istream& stream, Register & record){
  stream.read((char*) &record, sizeof(record));
  return stream;
}

ostream& operator<< (ostream& stream, Register & record){
  stream.write((char*) &record, sizeof(record));
  return stream;
}

istream& operator>> (istream& stream, Index & record){
  stream.read((char*) &record, sizeof(record));
  return stream;
}

ostream& operator<< (ostream& stream, Index & record){
  stream.write((char*) &record, sizeof(record));
  return stream;
}

ostream& operator<< (ostream& stream, Page & page){
  stream.write((char*) &page, sizeof(page));
  return stream;
}

istream& operator>> (istream& stream, Page & page){
  stream.read((char*) &page, sizeof(page));
  return stream;
}

void sort(vector<Index> &vec, int l, int m, int r){
  int i, j, k;
  int n1 = m - l + 1;
  int n2 = r - m; 
  vector<Index> L, R; 
  for (i = 0; i < n1; i++) 
      L.push_back(vec[l + i]);
  for (j = 0; j < n2; j++) 
      R.push_back(vec[m + 1 + j]); 

  i = 0;
  j = 0;
  k = l;
  while (i < n1 && j < n2) { 
      if (L[i].key <= R[j].key) { 
          vec[k] = L[i]; 
          i++; 
      } 
      else { 
          vec[k] = R[j]; 
          j++; 
      } 
      k++; 
  } 
  while (i < n1) { 
      vec[k] = L[i]; 
      i++; 
      k++; 
  } 
  while (j < n2) { 
      vec[k] = R[j]; 
      j++; 
      k++; 
  } 
}

void MergeSort(vector<Index> &vec, int l, int r){
  if(l < r){
    int temp = (l + r)/2;
    MergeSort(vec, l, temp);
    MergeSort(vec, temp + 1, r);
    sort(vec, l, temp, r);
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

class ISAM{
  private:
    string fileName;
    string indexName; // TO DO -> array of levels 
    vector<Index> index = {}; // diccionario en memoria principal

  public:

    void loadIndex(){ // TO DO -> array of levels 
      Index idx;
      ifstream in_idx(indexName, ios::binary); if (!in_idx.is_open()) return;
      while(in_idx >> idx)
        index.push_back(idx);
      in_idx.close();
    }

    ISAM(string _fileName, string csv = ""){
      fileName = _fileName;
      indexName = fileName.substr(0, fileName.length()-4) + "_index" + to_string(1) + ".dat";

      /*fstream file(fileName, ios::out | ios::in | ios::ate | ios::app | ios::binary);
      (file.is_open()) ? file.close() : throw("Unable to open files");*/
      /*fstream file2(fileName, ios::out | ios::in | ios::ate | ios::app | ios::binary);
      (file2.is_open()) ? file2.close() : throw("Unable to open files");*/ 

      // later check for file errors
      
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

        Register temp(name.c_str(), user.c_str(), mail.c_str(), pass.c_str());
        registers.push_back(temp);
      }

      sort(registers.begin(),registers.end(),reg_nom_comp);
      
      // write pages to data file 
      ofstream out(fileName, ios::app | ios::binary);
      vector<Register> records;
      for (auto i = 0; i < registers.size(); ++i){
        records.push_back(registers[i]);
        if (records.size() == PAGE_SIZE || i >= registers.size()){
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

    PageLocation search(string key){
      PageLocation empty;

      fstream datafile(fileName, ios::out | ios::in | ios::ate | ios::app | ios::binary);
      if(!datafile.is_open()) throw("Unable to open files");
      int temp = 0;
      int l = 0;
      int u = sizeof(index);
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
      if (temp >= index.size()) return empty;

      long address = index[temp].address;
      cout << address << endl;
      datafile.seekg(address);

      Page curr_page;
      datafile >> curr_page;
      auto iterator = curr_page;
      while (1){
        int i = 0;
        for (auto it : iterator.records) {
          if (it.name == key){
            PageLocation result(it, address, i);
            cout << key << " was found!" << endl;
            return result;
          }
          if (iterator.next_bucket != -1){
            datafile.seekg(iterator.next_bucket);
            datafile >> iterator;
          } else {
            cout << "Unable to locate key" << endl;
            return empty;
          }
          i++;
        }
      }
    }

    // void agregarRegistro(Register record){
    //   string key = record.codigo;
    //   if(index[key].i == -1){
    //     fstream file(fileName , ios::out | ios:: binary | ios::app);
    //     file.write((char*)&record, sizeof(record));
    //     index[key].i = (file.tellg()/(long)sizeof(record));
    //     file.close();
    //   }
    //   else{
    //     cout << "Code taken\n";
    //   }
    //   return;

    // }
};

int main(){
  ISAM ourISAM("Registro de Usuarios.dat", "Usuario.csv");
  
  auto resutl = ourISAM.search("a Fulton");
  
}
