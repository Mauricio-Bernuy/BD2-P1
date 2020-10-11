#include <iostream>
#include <pthread.h>
#include <string>
#include <fstream>
#include <vector>
#include <string.h>
#include <stack>
#include <algorithm>
#include <sstream>

using namespace std;


short get_operation(const char op[10]){
  short operation = 3;
  if (!strcmp(op, "SELECT")) operation = 0;
  else if (!strcmp(op, "INSERT")) operation = 1;
  else if (!strcmp(op, "DELETE")) operation = 2;
  return operation;
}

struct Query{
  short operation;
  string key;
  //struct Register reg;
  //Query(const char op[10], string key[30]){
  //};
  Query(string query1){
    stringstream query(query1);
    string q1;
    getline(query, q1, ' ');
    cout << "Query type: " << q1 << endl;
    operation = get_operation(q1.c_str());
    string strs[5];
    int temp = 0;
    switch(operation){
      case 0:
      case 2:
        cout << "0 or 1" << endl;
        while(q1 != "="){
          getline(query, q1, ' ');
        }
        getline(query, q1, ' ');
        q1 = q1.substr(1, q1.length()-3);
        cout << q1 << endl;
        break;
      case 1:
        while(q1 != "VALUES"){
          getline(query, q1, ' ');
        }
        cout << "Q1 ------: " << q1 << endl;
        getline(query, q1, ',');
        do{
          ++temp;
          cout << "char: " << q1[temp] << " ";
        }while(q1[temp] != 39);
        getline(query, q1, ' ');
        strs[0] = q1.substr(2, temp);
        getline(query, q1, ' ');
        for(int i = 1; i < 4; ++i){
        temp = 0;
        do{
          ++temp;
        }while(q1[temp] != 39);
        strs[i] = q1.substr(1, temp-1);
        getline(query, q1, ' ');
        }
        for(int i = 0; i < 4; ++i){
          cout << strs[i] << endl;
        }
        break;
      default:
        cout << "Invalid query" << endl;
        throw;
    }
  }
};

void execute_query(){

}


int main(){
  string query1("SELECT * FROM ISAM WHERE NOMBRE = 'Stephano';");
  string query2("INSERT INTO ISAM VALUES ('Stephano Wurt', 'Stephano_Wurt', 'stephano_wurt@mail.com', 'abcdefghijkl');");
  string query3("DELETE FROM ISAM WHERE NOMBRE = 'Stephano';");
  string query4("Ay lmao");
  //Query q1(query1);
  Query q2(query2);
  //Query q3(query3);
  //Query q4(query4);
  return 0;
}
