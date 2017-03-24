#include <stdio.h>
#include <string>
#include <defs.h>
#include <set>
#include <map>
#include <iostream>

using namespace std;

static map<LexemeType, string> types = {
  {LT_NONE ,"NONE"},
  {LT_IDENTIFICATE ,"IDENTIFICATOR"},
  {LT_SEPARATOR ,"SEPARATOR"},
  {LT_OPERATOR ,"OPERATOR"},
  {LT_SPACE ,"SPACE"},
  {LT_DIRECT ,"DIRECTIVE"},
  {LT_EOF ,"EOF"},
  {LT_FLOAT ,"LETERAL_FLOAT"},
  {LT_INT ,"LETERAL_INTEGER"},
  {LT_STRING ,"LETERAL_STRING"},
  {LT_VAR ,"VARIABLE"},
  {LT_ARRAY ,"ARRAY"},
  {LT_COMMENT ,"COMMENT"},
  {LT_KEY_WORD ,"KEY_WORD"},
  {LT_FUNCTION_NAME ,"FUNCTION_NAME"},
  {LT_TYPE_NAME ,"TYPE_NAME"},
};

set<string> key_words = {
  "type",
  "var",
  "def",
  "do",
  "end",
  "for",
  "if",
  "while",
  "ref",
  "return",
  "in",
  "record",
  "break",
  "continue",
  "else",
  "true",
  "false",
  "null"
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
  if (type == LT_IDENTIFICATE){
    if (key_words.find(str) != key_words.end()){
      type = LT_KEY_WORD;
    }
    else{
      type = LT_FUNCTION_NAME;
    }
  }
  return type;
}

string Lexeme::get_type_str(){
  return types[type];
}

string Lexeme::get_value(){
  stringstream ss;
  switch (get_type()){
    case LT_NONE:
      throw Errors::Unknown_lexeme(*this);
    case LT_FLOAT_AFTER_POINT:
      throw Errors::Unknown_lexeme(*this);
    case LT_SPACE:
      for (unsigned int i = 0; i < str.size(); i++){
        if (str[i] == '\n'){
          return "NewLine\n";
        }
      }
      return "Spaces";
    case LT_DIRECT:
    case LT_EOF:
      return "EndOfFile";
    case LT_LETERAL:
      // type = LT_INT;
    case LT_STRING:
    case LT_FLOAT:
    case LT_INT:
    case LT_SEPARATOR:
    case LT_OPERATOR:
    case LT_VAR:
    case LT_ARRAY:
    case LT_COMMENT:
    case LT_IDENTIFICATE:
    case LT_KEY_WORD:
    case LT_FUNCTION_NAME:
    default:
      return str;
  }
}

bool Lexeme::is_leteral(){
  switch(type){
    case LT_INT:
    case LT_FLOAT:
    case LT_STRING:
      return true;
    default:
      return false;
  }
}

bool Lexeme::is_identificator(){
  switch(type){
    case LT_VAR:
    case LT_ARRAY:
    case LT_FUNCTION_NAME:
    case LT_TYPE_NAME:
      return true;
    default:
      return false;
  }
}


bool Lexeme::operator==(const LexemeType& ltype) const{
  return type == ltype;
}


void Lexeme::print(){
  printf("%d %d %s %s\n", row+1, col+1, types[type].c_str(), get_value().c_str());
}
