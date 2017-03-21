#include <lexicalSearch.h>
#include <sstream>
#include <iostream>
using namespace std;



Searcher::Searcher(string file_name){
  input_f.open(file_name);

  input_f.seekg (0, input_f.end);
  buf_size = input_f.tellg();
  input_f.seekg (0, input_f.beg);
  buffer = new char[buf_size];
  current_ind = 0;
}

Searcher::~Searcher(){
  delete[] buffer;
  input_f.close();
}

Lexeme Searcher::next(){
  current = get_next();
  while (current.get_value() == "Spaces"){
    current = get_next();
  }
  return current;
}

Lexeme Searcher::get_next(){
  Lexeme t;
  if (!q.empty()){
    t = q.front();
    q.pop();
    return t;
  }
  try{
    t = m.next();
  }
  catch(Errors::End_of_line e){
    next_line();
    return e.last_lexeme;
  }
  if (t.get_type() == TYPE_FLOAT_POINT){
    string s = t.get_str();
    unsigned int k = 0;
    for (unsigned int i = 0; i < s.size(); i++){
      if (s[i] == '.'){
        k = i;
        break;
      }
    }
    if (s.size() - k > 3){
      throw Errors::Unknown_lexeme(t);
    }
    unsigned int lim = s.size() - k;
    for (unsigned int i = 0; i < lim; i++){
      s.pop_back();
    }
    // cout << "POP " << lim << " " << string(lim, '.') << endl;
    Lexeme integer(s, t.get_row(), t.get_col(), TYPE_INT);
    Lexeme point(string(lim, '.'), t.get_row(), t.get_col() + k, TYPE_OPERATOR);
    q.push(point);
    return integer;
  }
  return t;
}

Lexeme Searcher::get_current(){
  return current;
}

void Searcher::learn(){
  m.learn();
  refresh_buffer();
  next_line();
}

void Searcher::refresh_buffer(){
  input_f.read(buffer, buf_size);
  current_ind = 0;
  // for (int i = 0; i < 10; i++){
  //   cout << (int)buffer[i] << endl;
  // }
}

void Searcher::next_line(){
  // cout << "next_line " << current_ind << endl;
  int& ind = current_ind;
  int i = ind;
  string s;
  while (true){
    if (buffer[i] == '\n' || buffer[i] == 3){
      i++;
      s = s + string(&buffer[ind], i - ind);
      break;
    }
    if (i + 1 >= buf_size){
      s = s + string(1, (char)3);
    }
    i++;
  }
  ind = i;
  m.add_str(s);
}
