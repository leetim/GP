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
    try{
      while (true){
        s.next().print();
      }
    }
    catch(Lexeme e){
      e.print();
    }
  }
  catch(Errors::Unknown_lexeme e1){
    printf("Unknown_lexeme %d %d '%s'\n", e1.lex.get_row(), e1.lex.get_col(), e1.lex.get_str().c_str());
    return 1;
  }
  catch(...){
    printf("Unknown error\n");
  }
  return 0;
}
