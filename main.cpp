#include <iostream>
#include <lexicalSearch.h>
#include <fstream>
#include <string>
#define DEF_STR "input.gp"

using namespace std;

char s[300];
int n = 300;

int main(int argc, char** argv){
  string str;
  if (argc > 1){
    str = string(argv[1]);
  }
  else{
    str = string(DEF_STR);
  }
  Searcher s(str);
  // cout << "123" << endl;
  s.learn();
  try{
    while (true){
      s.next().print();
    }
  }
  catch(Lexeme e){
    e.print();
  }
  return 0;
}
