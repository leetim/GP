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
  {LT_NEW_LINE ,"SPACE"},
  {LT_DIRECT ,"DIRECTIVE"},
  {LT_EOF ,"EOF"},
  {LT_FLOAT ,"LETERAL_FLOAT"},
  {LT_INT ,"LETERAL_INTEGER"},
  {LT_STRING ,"LETERAL_STRING"},
  {LT_VARIABLE ,"VARIABLE"},
  {LT_ARRAY ,"ARRAY"},
  {LT_COMMENT ,"COMMENT"},
  {LT_KEY_WORD ,"KEY_WORD"},
  {LT_FUNCTION_NAME ,"FUNCTION_NAME"},
  {LT_TYPE_NAME ,"TYPE_NAME"},
  {LT_OP, "SEPARATOR"},
  {LT_CP, "SEPARATOR"},
  {LT_OSB, "SEPARATOR"},
  {LT_CSB, "SEPARATOR"},
  {LT_OB, "SEPARATOR"},
  {LT_CB, "SEPARATOR"},
  {LT_TYPE, "KEY_WORD"},
  {LT_VAR, "KEY_WORD"},
  {LT_DEF, "KEY_WORD"},
  {LT_DO, "KEY_WORD"},
  {LT_END, "KEY_WORD"},
  {LT_FOR, "KEY_WORD"},
  {LT_IF, "KEY_WORD"},
  {LT_WHILE, "KEY_WORD"},
  {LT_REF, "KEY_WORD"},
  {LT_RETURN, "KEY_WORD"},
  {LT_IN, "KEY_WORD"},
  {LT_RECORD, "KEY_WORD"},
  {LT_BREAK, "KEY_WORD"},
  {LT_CONTINUE, "KEY_WORD"},
  {LT_END, "KEY_WORD"},
  {LT_TRUE, "KEY_WORD"},
  {LT_FALSE, "KEY_WORD"},
  {LT_NULL, "KEY_WORD"},
  {LT_LET, "KEY_WORD"},
  {LT_ARROW, "OPERATOR"},
  {LT_ASSIGMENT, "OPERATOR"},
  {LT_COLON, "OPERATOR"},
  {LT_ELSE, "KEY_WORD"},
  {LT_COMMA, "COMMA"},
  {LT_RANGE, "OPERATOR"},
  {LT_OPEN_RANGE, "OPERATOR"},
  {LT_PUTS, "KEY_WORD"},
  {LT_PRINT, "KEY_WORD"},
  {LT_ARRAY_ELEMENT, "ARRAY_ELEMENT"},
  {LT_CLASS, "KEY_WORD"},

  {LT_ADD, "OPERATOR"},
  {LT_SUB, "OPERATOR"},
  {LT_MULT, "OPERATOR"},
  {LT_DIV, "OPERATOR"},
  {LT_MOD, "OPERATOR"},
  {LT_NOT, "OPERATOR"},
  {LT_BIN_AND, "OPERATOR"},
  {LT_BIN_OR, "OPERATOR"},
  {LT_BIN_XOR, "OPERATOR"},
  {LT_LOG_AND, "OPERATOR"},
  {LT_LOG_OR, "OPERATOR"},
  {LT_LOG_XOR, "OPERATOR"},
  {LT_EQ, "OPERATOR"},
  {LT_NEQ, "OPERATOR"},
  {LT_LT, "OPERATOR"},
  {LT_LTEQ, "OPERATOR"},
  {LT_GT, "OPERATOR"},
  {LT_GTEQ, "OPERATOR"},
  {LT_SHL, "OPERATOR"},
  {LT_SHR, "OPERATOR"},
  {LT_POINT, "OPERATOR"}
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
  // "record",
  "break",
  "continue",
  "else",
  "true",
  "false",
  "null",
  "let",
  "else",
  "puts",
  "print",
  "class"
};

map<string, LexemeType> types_key_words = {
  {"(", LT_OP},
  {")", LT_CP},
  {"[", LT_OSB},
  {"]", LT_CSB},
  {"{", LT_OB},
  {"}", LT_CB},
  {":", LT_COLON},
  {"type", LT_TYPE},
  {"var", LT_VAR},
  {"def", LT_DEF},
  {"do", LT_DO},
  {"end", LT_END},
  {"for", LT_FOR},
  {"if", LT_IF},
  {"while", LT_WHILE},
  {"ref", LT_REF},
  {"return", LT_RETURN},
  {"in", LT_IN},
  // {"record", LT_RECORD},
  {"break", LT_BREAK},
  {"continue", LT_CONTINUE},
  {"end", LT_END},
  {"true", LT_TRUE},
  {"false", LT_FALSE},
  {"null", LT_NULL},
  {"let", LT_LET},
  {"else", LT_ELSE},
  {"->", LT_ARROW},
  {"=", LT_ASSIGMENT},
  // {"+=", LT_ASSIGMENT},
  // {"-=", LT_ASSIGMENT},
  // {"/=", LT_ASSIGMENT},
  // {"*=", LT_ASSIGMENT},
  // {"%=", LT_ASSIGMENT},
  // {"|=", LT_ASSIGMENT},
  // {"&=", LT_ASSIGMENT},
  // {"^=", LT_ASSIGMENT},
  {",", LT_COMMA},
  {"...", LT_RANGE},
  {"..", LT_OPEN_RANGE},
  {"puts", LT_PUTS},
  {"print", LT_PRINT},
  {"class", LT_CLASS},

  {"+", LT_ADD},
  {"-", LT_SUB},
  {"*", LT_MULT},
  {"/", LT_DIV},
  {"%", LT_MOD},
  {"!", LT_NOT},
  {"&", LT_BIN_AND},
  {"|", LT_BIN_OR},
  {"^", LT_BIN_XOR},
  {"&&", LT_LOG_AND},
  {"||", LT_LOG_OR},
  // {"", LT_LOG_XOR},
  {"==", LT_EQ},
  {"!=", LT_NEQ},
  {"<", LT_LT},
  {"<=", LT_LTEQ},
  {">", LT_GT},
  {">=", LT_GTEQ},
  {"<<", LT_SHL},
  {">>", LT_SHR},
  {".", LT_POINT}
};

////////////////////////////////////////////////////////////////////////////////
//Lexeme

string& Lexeme::get_str(){
  return str;
}

int Lexeme::get_row(){
  return row;
}

int Lexeme::get_col(){
  return col;
}

int Lexeme::get_type(){
  if (types_key_words[get_str()]){
    type = types_key_words[get_str()];
  }
  if (type == LT_IDENTIFICATE){
    type = LT_FUNCTION_NAME;
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
      return "Spaces";
    case LT_NEW_LINE:
      return "NewLine\n";
    case LT_DIRECT:
    case LT_EOF:
      return "EndOfFile";
    case LT_LETERAL:
      // type = LT_INT;
    case LT_STRING:
    case LT_FLOAT:
    case LT_INT:
    case LT_OPERATOR:
    case LT_VARIABLE:
    case LT_ARRAY:
    case LT_COMMENT:
    case LT_IDENTIFICATE:
    case LT_KEY_WORD:
    case LT_FUNCTION_NAME:
    case LT_SEPARATOR:
    default:
      return str;
  }
}

bool Lexeme::is_leteral(){
  switch(type){
    case LT_INT:
    case LT_FLOAT:
    case LT_STRING:
    case LT_TRUE:
    case LT_FALSE:
      return true;
    default:
      return false;
  }
}

bool Lexeme::is_identificator(){
  switch(type){
    case LT_VARIABLE:
    case LT_ARRAY:
    case LT_FUNCTION_NAME:
    case LT_TYPE_NAME:
      return true;
    default:
      return false;
  }
}

bool Lexeme::is_separator(){
  switch(type){
    case LT_OP:
    case LT_CP:
    case LT_OSB:
    case LT_CSB:
    case LT_OB:
    case LT_CB:
      return true;
    default:
      return false;
  }
}

bool Lexeme::is_close_separator(){
  switch(type){
    case LT_CP:
    case LT_CSB:
    case LT_CB:
      return true;
    default:
      return false;
  }
}

bool Lexeme::operator==(const LexemeType& ltype) const{
  return type == ltype;
}

bool Lexeme::operator!=(const LexemeType& lt) const{
  return type != lt;
}



void Lexeme::print(){
  printf("%d %d %s %s\n", row+1, col+1, types[type].c_str(), get_value().c_str());
}
