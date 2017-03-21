#include <stdio.h>
#include <string>
#include <defs.h>
#include <set>
#include <map>
#include <iostream>

using namespace std;

static map<LexemeType, string> types = {
  {TYPE_NONE ,"NONE"},
  {TYPE_IDENTIFICATE ,"IDENTIFICATOR"},
  {TYPE_SEPARATOR ,"SEPARATOR"},
  {TYPE_OPERATOR ,"OPERATOR"},
  {TYPE_SPACE ,"SPACE"},
  {TYPE_DIRECT ,"DIRECTIVE"},
  {TYPE_EOF ,"EOF"},
  {TYPE_FLOAT ,"LETERAL_FLOAT"},
  {TYPE_INT ,"LETERAL_INTEGER"},
  {TYPE_STRING ,"LETERAL_STRING"},
  {TYPE_VAR ,"VARIABLE"},
  {TYPE_ARRAY ,"ARRAY"},
  {TYPE_COMMENT ,"COMMENT"},
  {TYPE_KEY_WORD ,"KEY_WORD"},
  {TYPE_FUNCTION_NAME ,"FUNCTION_NAME"},
  {TYPE_TYPE_NAME ,"TYPE_NAME"},
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
  if (type == TYPE_IDENTIFICATE){
    if (key_words.find(str) != key_words.end()){
      type = TYPE_KEY_WORD;
    }
    else{
      type = TYPE_FUNCTION_NAME;
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
    case TYPE_EOF:
      return "EndOfFile";
    case TYPE_LETERAL:
      // type = TYPE_INT;
    case TYPE_STRING:
    case TYPE_FLOAT:
    case TYPE_INT:
    case TYPE_SEPARATOR:
    case TYPE_OPERATOR:
    case TYPE_VAR:
    case TYPE_ARRAY:
    case TYPE_COMMENT:
    case TYPE_IDENTIFICATE:
    case TYPE_KEY_WORD:
    case TYPE_FUNCTION_NAME:
    default:
      return str;
  }
}

bool Lexeme::operator==(const LexemeType& ltype) const{
  return type == ltype;
}


void Lexeme::print(){
  printf("%d %d %s %s\n", row+1, col+1, types[type].c_str(), get_value().c_str());
}
