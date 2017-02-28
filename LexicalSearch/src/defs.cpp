#include <stdio.h>
#include <string>
#include <defs.h>

using namespace std;

static std::string types[] = {
  "NONE",
  "IDENTIFICATOR",
  "LETERAL_INTEGER",
  "SEPARATOR",
  "OPERATOR",
  "SPACE",
  "DIRECTIVE",
  "EOF",
  "LETERAL_FLOAT",
  "LETERAL_INTEGER",
  "LETERAL_STRING",
  "VARIABLE",
  "ARRAY",
  "COMMENT"
};

////////////////////////////////////////////////////////////////////////////////
//Lexeme

string Lexeme::get_str(){
  return str;
}

int Lexeme::get_row(){
  return row;
}

int Lexeme::get_col(){
  return col;
}

int Lexeme::get_type(){
  return type;
}

string Lexeme::get_type_str(){
  return types[type];
}

string Lexeme::get_value(){
  stringstream ss;
  switch (type){
    case TYPE_NONE:
      throw Errors::Unknown_lexeme(*this);
    case TYPE_FLOAT_AFTER_POINT:
      throw Errors::Unknown_lexeme(*this);
    case TYPE_SPACE:
      for (unsigned int i = 0; i < str.size(); i++){
        if (str[i] == '\n'){
          return "NewLine\n";
        }
      }
      return "Spaces";
    case TYPE_DIRECT:
    case TYPE_STRING:
      return "'" + str + "'";
    case TYPE_EOF:
      return "EndOfFile";
    case TYPE_LETERAL:
      // type = TYPE_INT;
    case TYPE_SEPARATOR:
    case TYPE_OPERATOR:
    case TYPE_FLOAT:
    case TYPE_INT:
    case TYPE_VAR:
    case TYPE_ARRAY:
    case TYPE_COMMENT:
    case TYPE_IDENTIFICATE:
    default:
      return str;
  }
}

void Lexeme::print(){
  printf("%s %d %d %s\n", types[type].c_str(), row+1, col+1, get_value().c_str());
}
