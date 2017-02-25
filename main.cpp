#include <iostream>
#include <lexicalSearch.h>
#include <fstream>
#include <string>

using namespace std;

char s[300];
int n = 300;

int main(int argc, char** argv){
  // Searcher s;
  ifstream input("input.gp");
  input.read(s, n);

  Machine m;
  m.learn();
  m.add_str("  for $i in [0..10] do\n");
  try{
    for (int i = 0; i < 15 ; i++){
      // cout << "123" << endl;
      m.next().print();
    }
  }
  catch(int e){
    cout << e << endl;
  }
  return 0;
}
