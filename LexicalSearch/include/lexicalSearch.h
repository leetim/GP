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
  std::string current_line;
  std::queue<Lexeme> q;
  std::ifstream input_f;
  Machine m;
};
