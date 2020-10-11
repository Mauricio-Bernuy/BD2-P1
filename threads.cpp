#include <iostream>
#include <thread>
#include <string>
#include <fstream>
#include <vector>
#include <string.h>
#include <stack>
#include <algorithm>
#include <sstream>

using namespace std;

struct Query{
  short operation;
  string key;
  Register reg;
};

void execute_query(Query query, short lock){
  switch(query.operation){
    case 0:
      //search
      PageLocation pg = search(query.key, lock);
    case 1:
      //insert
      if (insert(query.reg, lock)) cout << "Inserted!" << endl;
      else cout << "Insertion invalid" << endl;
      break;
    case 2:
      //delete
      if (erase(query.reg, lock)) cout << "Erased!" << endl;
      else cout << "Erase invalid" << endl;
      break;
    default:
      cout << "Invalid query" << endl;
      throw;
  }
};

void order(Query q1, Query q2){
  short lock = 0;
  short op1 = q1.operation;
  short op2 = q2.operation;
  short op_flag = op1 + op2;
  Query lowest = (op1 < op2) ? q1 : q2;
  Query biggest = (op1 > op2) ? q2 : q1;
  if(op1 == 3 || op2 == 3){ cout << "Query invalid" << endl; throw; }
  if(op_flag == 0){
    if(!strcmp(q1.key == q2.key)) lock = 1;
    if(!strcmp(lowest.key, index[0].key)) lock = 2;
  }
  else{
    if(op_flag == 3) lock = 1;
    else{
      if(op_flag != -1) {
        if(!strcmp(q1.key == q2.key)){
          pthread_t qu2(execute_query, q1, lock);
          qu2.join();
          return;
        };
        if(!strcmp(lowest.key, index[0].key)) lock = 2;
      };
      else{
        lock = 2;
        pthread_t qu1(execute_query, q1, 0);
        pthread_t qu2(execute_query, q2, lock);
        qu1.join();
        qu2.join();
      }
    }
  }
  pthread_t qu1(execute_query, lowest, 0);
  pthread_t qu2(execute_query, biggest, lock);
  qu1.join();
  qu2.join();
  return;
}


int main(){
  return 0;
}
