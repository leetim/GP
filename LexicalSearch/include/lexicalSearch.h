#pragma once
#include <string>
#include <defs.h>
#include <vector>
#include <machine.h>

class Searcher{
public:
  Searcher(){};
  Searcher(std::string file_name);
  Lexeme getNextLexema();
  void learn();
private:
  void next_line();
  std::string current_line;
  // int getInc(int a);
};
