#include<iostream>
#include<string.h>
#include<map>
#include<fstream>
#include<cstdio>
#include<vector>
#include <bits/stdc++.h> 
#include <algorithm>

using namespace std;

#define INDEX_SIZE 4
#define PAGE_SIZE 4
#define INDEX_LEVELS 1

struct Register{
  char name[30];
  char user[30];
  char mail[41];
  char pass[12];

  Register(string nam, string usr, string ml, string pss){
      nam = nam.substr(0,29);
      usr = usr.substr(0,29);
      ml = ml.substr(0,40); 
      pss = pss.substr(0,11);      

      strcpy(name, (nam + string(30 - nam.length() ,' ')).c_str());
      cout << "Defined name as " << name << endl;
      strcpy(user, (usr + string(30 - usr.length() ,' ')).c_str());
      cout << "Defined user as " << user << endl;
      strcpy(mail, (ml + string(41 - ml.length() ,' ')).c_str());
      cout << "Defined mail as " << mail << endl;
      strcpy(pass, (pss + string(12 - pss.length() ,' ')).c_str());
      cout << "Definned pass as " << pass << endl;
  }
  Register(){};
  void print(){
    cout << "------------------------------"<<endl;
    cout << "Nombre: " << string(name).substr(0,29) << endl;
    cout << "Usuario: " << string(user).substr(0,29) << endl;
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

struct IndexLvl{
    long address;
    char index[30];
};

struct Index{
    long address;
    char index[30];
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
      if (L[i].index <= R[j].index) { 
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

// template<class T>
// istream& operator>> (istream& stream, T & record){
//   stream.read((char*) &record, sizeof(record));
//   return stream;
// }
// template<class T>
// ostream& operator<< (ostream& stream, T & record){
//   stream.write((char*) &record, sizeof(record));
//   return stream;
// }

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


void offload(vector<Register> vectout, string filename){
  ofstream out(filename, ios::app | ios::binary);

  vector<Register> records;
  for (auto i = 0; i < vectout.size(); ++i){
    
    records.push_back(vectout[i]);

    if (records.size() == PAGE_SIZE || i >= vectout.size()){
      Page pageout(records);

      //out << pageout;

      out.write((char*) &pageout, sizeof(pageout));
      records.clear();
    }
  }
  out.close();
}

class ISAM{
  private:
    string fileName;
    string indexName;
    int init_size;
    int index_size = INDEX_SIZE;
    vector<Index> index = {}; //diccionario en memoria principal

  public:
    vector<Index> leerIndice(){
      Index index;
      vector<Index> ind = {};
      fstream ind_file(indexName, ios::out | ios::in | ios::ate | ios::app | ios::binary);
      if (ind_file.is_open()){
        long ind_size = 0;
        string location = "";
        ind_file.seekg(0, ios::end);
        init_size = 0;
        ind_size = ind_file.tellg();
        if(ind_size == 0) {ind_file.close(); return ind;};
        ind_file.seekg(0);
				while(ind_file >> index){ 
          ++init_size;
          ind.push_back(index);
        }
        ind_file.close();
      }
      return ind;
    }

    void parseIndex(){
      fstream data_file(fileName, ios::out | ios::in | ios::ate | ios::app | ios::binary);
      if(data_file.is_open()){
        fstream index_file(fileName, ios::out | ios::in | ios::ate | ios::app | ios::binary);
        if(index_file.is_open()){
          data_file.seekg(0, ios::end);
          long data_size = data_file.tellg();
          long data_page_num = data_size / sizeof(Page);
          Index ind;
          Register temp;
          long position;
          for(long i = 0; i < INDEX_SIZE ; ++i){
            position = i*sizeof(Page);
            ind.address = position;
            data_file.seekg(position, ios::beg);
            data_file >> temp;
            strcpy(ind.index, temp.name);
            index.push_back(ind);
          }
          MergeSort(index, 0, INDEX_SIZE);
          index_file.close();
        }
        data_file.close();        
      }
    }

    void guardarIndices(){
      ofstream index_file(indexName, ios::out | ios_base::app);
      for(auto it= index.begin(); it != index.end(); ++it){
					index_file << *it;
      }
      index_file.close();
    }


    ISAM(string _fileName){
      fileName = _fileName;
      fstream file(fileName, ios::out | ios::in | ios::ate | ios::app | ios::binary);
      (file.is_open()) ? file.close() : throw("Unable to open files");
      indexName = "index_"+_fileName;
      fstream file2(fileName, ios::out | ios::in | ios::ate | ios::app | ios::binary);
      (file2.is_open()) ? file2.close() : throw("Unable to open files");
      index = leerIndice();
      if(index.empty()){
        parseIndex();
        guardarIndices();
      }
    }

    ~ISAM(){
    }

    Register buscar(string key){
      int temp = 0;
      int l = 0;
      int u = sizeof(index);
      while (u >= l){
        temp = (l+u)/2;
        if (key < index[temp].index) u = temp - 1;
        else{
          if (key > index[temp].index) l = temp + 1; else break;
        }
      }
      
      
      
      // long ind = index[key].i;
      // Register response;
      // if(ind!=-1){
      //   fstream file(fileName, ios::in | ios:: binary);
      //   file.seekg((index[key].i-1)*sizeof(response));
      //   file >> response;
      // }
      // else{strcpy(response.codigo,"NA");strcpy(response.nombre,"Not found");}
			// return response;
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

  load_data();


  offload(registers, "test.dat");

  ifstream pepito("test.dat", ios::binary);
  Page pepe;
  vector<Page> peppa;

  while(pepito >> pepe){
    peppa.push_back(pepe);
  };

  //ISAM structure("datos1.txt");
  Register registro1((char*)"Ana", (char*)"ana1", (char*)"i1l.com", (char*)"abcdefg");
  Register registro2((char*)"Beto", (char*)"beto1", (char*)"mail2.com", (char*)"abcdefg");
  Register registro3((char*)"Camila", (char*)"cam1", (char*)"mail3.com", (char*)"abcdefg");
  Register registro4((char*)"Stephano", (char*)"st", (char*)"mail4.com", (char*)"abcdefg");

	vector<Register> regs;
  regs.push_back(registro1);
  regs.push_back(registro2);
  regs.push_back(registro3);
  regs.push_back(registro4);

  int temp = 0;
  int l = 0;
  int u = regs.size();
  string key = "Stephano";
  while (u >= l){
    cout << "In while ";
    regs[temp].print();
    temp = (l+u)/2;
    if (key < regs[temp].name) u = temp - 1;
    else{
      if (key > regs[temp].name) l = temp + 1; else break;
    }
  }
  //cout << endl << endl << regs[temp].name << endl;
  


	// for(int i =0; i < 3; ++i){
  //   structure.agregarRegistro(regs[i]);
  // }
  // structure.agregarRegistro(registro4);
	// Register buscado = rf.buscar("0005");
	// buscado.print();
  return 0;
}
