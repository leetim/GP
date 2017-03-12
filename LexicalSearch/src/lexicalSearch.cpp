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
  Lexeme t;
  if (!q.empty()){
    t = q.front();
    q.pop();
    return t;
  }
  while (true){
    try{
      t = m.next();
    }
    catch(Lexeme e){
      if (e.get_type() == TYPE_EOF){
        // printf("EOF FOUNDED\n");
        throw e;
      }
      if (!q.empty()){
        t = q.front();
        q.pop();
        q.push(e);
        e = t;
      }
      next_line();
      return e;
    }
    if (t.get_type() == TYPE_NONE || t.get_type() == TYPE_EOF){
      throw t;
    }
    if (t.get_type() == TYPE_FLOAT_AFTER_POINT && q.empty()){
      throw t;
    }
    if (t.get_type() != TYPE_LETERAL && q.empty()){
      return t;
    }
    // cout << "PUSH " << t.get_str() << endl;
    q.push(t);
    if (q.size() == 3){
      Lexeme t1, t2, t3;
      t1 = q.front(); q.pop();
      t2 = q.front(); q.pop();
      t3 = q.front(); q.pop();
      if (t2.get_str() == "." && (t3.get_type() == TYPE_LETERAL || t3.get_type() == TYPE_FLOAT_AFTER_POINT)){
        string str = t1.get_str() + t2.get_str() + t3.get_str();
        return Lexeme(str, t1.get_row(), t1.get_col(), TYPE_FLOAT);
      }
      else{
        q.push(t2);
        q.push(t3);
        return t1;
      }
    }
  }
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
