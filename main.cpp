#include <iostream>
#include <lexicalSearch.h>
#include <fstream>
#include <string>

using namespace std;

char s[300];
int n = 300;

int main(int argc, char** argv){
  Searcher s("input.gp");
  cout << "123" << endl;
  s.learn();
  try{
    for (int i = 0; i < 300; i++){
      s.next().print();
    }
  }
  catch(Lexeme e){
    e.print();
  }
  // ifstream input("input.gp");
  // input.read(s, n);

  // Machine m;
  // m.learn();
  // m.add_str("  for $i in #[0..10] do\n ");
  // try{
  //   for (int i = 0; i < 15 ; i++){
  //     m.next().print();
  //   }
  // }
  // catch(int e){
  //   cout << e << endl;
  // }
  return 0;
}
