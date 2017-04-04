#include <iostream>
#include <lexicalSearch.h>
#include <syntaxis_analyser.h>
#include <fstream>
#include <string>
#define DEF_STR "input.gp"

using namespace std;

char s[300];
int n = 300;

void lexer(int argc, char** argv){
  string str;
  if (argc > 1){
    str = string(argv[1]);
  }
  else{
    str = string(DEF_STR);
  }
  Searcher s(str);
  // s.learn();
  try{
    try{
      while (true){
        s.next().print();
      }
    }
    catch(Errors::End_of_file e){
      e.last_lexeme.print();
    }
  }
  catch(Errors::Unknown_lexeme e1){
    printf("Unknown_lexeme %d %d '%s'\n", e1.lex.get_row(), e1.lex.get_col(), e1.lex.get_str().c_str());
    // return 1;
  }
  catch(...){
    printf("Unknown error\n");
  }
}

int main(int argc, char** argv){
  // lexer(argc, argv);
  string str;
  if (argc > 1){
    str = string(argv[1]);
  }
  else{
    str = string(DEF_STR);
  }
  SyntaxisAnalyser sa(str);
  try{
    PNocle pn = sa.get_tree();
    cout << pn->get_str() << endl;
  }
  catch(Errors::Illegal_expression e){
    e.print();
  }
  catch(Errors::ClosingParenthesisNotFound e){
    e.print();
  }
  return 0;
}
