#pragma once
#include <string>
#include <vector>
#include <sstream>


enum LexemeType{
  TYPE_NONE,
  TYPE_IDENTIFICATE,
  TYPE_LETERAL,
  TYPE_SEPARATOR,
  TYPE_OPERATOR,
  TYPE_SPACE,
  TYPE_DIRECT,
  TYPE_EOF,
  TYPE_FLOAT,
  TYPE_INT,
  TYPE_STRING,
  TYPE_VAR,
  TYPE_ARRAY,
  TYPE_COMMENT,
  TYPE_FLOAT_AFTER_POINT,
  TYPE_FLOAT_POINT,
  TYPE_ACTIVE_STRING,
  TYPE_KEY_WORD,
  TYPE_FUNCTION_NAME,
  TYPE_TYPE_NAME
};

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
  Lexeme(std::string s, int _row, int _col, LexemeType _type): str(s), row(_row), col(_col), type(_type){};
  std::string get_str();
  int get_row();
  int get_col();
  int get_type();
  std::string get_type_str();
  std::string get_value();
  void print();
  bool operator==(const LexemeType&) const;
private:
  std::string str;
  int row;
  int col;
  LexemeType type;
};

namespace Errors{
  struct End_of_line{
    End_of_line(){};
    End_of_line(Lexeme l): last_lexeme(l){};
    Lexeme last_lexeme;
  };

  struct End_of_file{
    End_of_file(){};
    End_of_file(Lexeme l): last_lexeme(l){};
    Lexeme last_lexeme;
  };


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
