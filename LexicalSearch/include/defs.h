#pragma once
#include <string>
#include <vector>
#include <sstream>

#define TYPE_NONE 0
#define TYPE_IDENTIFICATE 1
#define TYPE_LETERAL 2
#define TYPE_SEPARATOR 3
#define TYPE_OPERATOR 4
#define TYPE_SPACE 5
#define TYPE_DIRECT 6
#define TYPE_EOF 7
#define TYPE_FLOAT 8
#define TYPE_INT 9
#define TYPE_STRING 10
#define TYPE_VAR 11
#define TYPE_ARRAY 12
#define TYPE_COMMENT 13
#define TYPE_FLOAT_AFTER_POINT 14

// std::vector<std::string> types;

template<class T>
void value_from_string(std::string target, T& result){
  std::stringstream ss;
  ss << target;
  ss >> result;
}



class Lexeme{
public:
  Lexeme(): str(), row(), col(), type(){};
  Lexeme(std::string s, int _row, int _col, int _type): str(s), row(_row), col(_col), type(_type){};
  std::string get_str();
  int get_row();
  int get_col();
  int get_type();
  std::string get_value();
  void print();
private:
  std::string str;
  int row;
  int col;
  int type;
};

namespace Errors{
  struct Unknown_symbol{
    Unknown_symbol(){};
    Unknown_symbol(char _c, int _row, int _col): c(_c), row(_row), col(_col){};
    char c;
    int row;
    int col;
  };

  struct Unknown_lexeme{
    Unknown_lexeme(){};
    Unknown_lexeme(Lexeme l): lex(l){};
    Lexeme lex;
  };
};
