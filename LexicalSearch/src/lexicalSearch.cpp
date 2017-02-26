#include <lexicalSearch.h>

using namespace std;


Searcher::Searcher(string file_name){
  input_f.open(file_name);
}

Searcher::~Searcher(){
  input_f.close();
}

Lexeme Searcher::next(){
  Lexeme t;
  while (true){
    if (t.get_type() == TYPE_LETERAL && q.empty()){
      q.push(t);
    }
  }
}

void Searcher::learn(){

}

void Searcher::next_line(){

}
