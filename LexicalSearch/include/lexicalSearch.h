#pragma once
#include <string>
#include <defs.h>
#include <vector>

class Lexeme{
public:
  Lexeme(): str(), row(), col(), type(){};
  Lexeme(std::string s, int _row, int _col, int _type): str(s), row(_row), col(_col), type(_type){};
  std::string print();
private:
  std::string str;
  int row;
  int col;
  int type;
};

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
