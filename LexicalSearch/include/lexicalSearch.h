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
  Lexeme next_with_spaces();
  Lexeme get_current();
private:
  void learn();
  void next_line();
  void refresh_buffer();
  Lexeme get_next();
  std::string current_line;
  std::queue<Lexeme> q;
  std::ifstream input_f;
  char* buffer;
  int buf_size;
  int current_ind;
  Machine m;
  Lexeme current;
};
