#pragma once
#include <string>
#include <defs.h>
#include <vector>
#include <machine.h>
#include <set>
#include <queue>
#include <fstream>


class Searcher{
public:
  Searcher(){};
  Searcher(std::string file_name);
  ~Searcher();
  Lexeme next();
  void learn();
private:
  void next_line();
  void refresh_buffer();
  std::string current_line;
  std::queue<Lexeme> q;
  std::ifstream input_f;
  char* buffer;
  int buf_size;
  int current_ind;
  Machine m;
};
