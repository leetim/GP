#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <sstream>

enum LexemeType{
  LT_NONE,
  LT_IDENTIFICATE,
  LT_LETERAL,
  LT_SEPARATOR,
  LT_OPERATOR,
  LT_SPACE,
  LT_DIRECT,
  LT_EOF,
  LT_FLOAT,
  LT_INT,
  LT_STRING,
  LT_VARIABLE,
  LT_ARRAY,
  LT_COMMENT,
  LT_FLOAT_AFTER_POINT,
  LT_FLOAT_POINT,
  LT_ACTIVE_STRING,
  LT_KEY_WORD,
  LT_FUNCTION_NAME,
  LT_TYPE_NAME,
  LT_NEW_LINE,
  LT_OP,//Открывающая круглая скобка
  LT_CP,
  LT_OSB,//Открывающая квадратная скобка
  LT_CSB,
  LT_OB,//Открывающая фигурная скобка
  LT_CB,
  LT_TYPE,
  LT_VAR,
  LT_DEF,
  LT_DO,
  LT_FOR,
  LT_IF,
  LT_ELSE,
  LT_WHILE,
  LT_REF,
  LT_RETURN,
  LT_IN,
  LT_RECORD,
  LT_BREAK,
  LT_CONTINUE,
  LT_END,
  LT_TRUE,
  LT_FALSE,
  LT_NULL,
  LT_LET,
  LT_ARROW,
  LT_ASSIGMENT,
  LT_COLON,
  LT_COMMA,
  LT_RANGE,
  LT_OPEN_RANGE,
  LT_PUTS,
  LT_PRINT,
  LT_ARRAY_ELEMENT,
  LT_CLASS,

  LT_ADD,
  LT_SUB,
  LT_MULT,
  LT_DIV,
  LT_MOD,

  LT_NOT,
  LT_BIN_AND,
  LT_BIN_OR,
  LT_BIN_XOR,
  LT_LOG_AND,
  LT_LOG_OR,
  LT_LOG_XOR,
  LT_EQ,
  LT_NEQ,
  LT_LT,
  LT_LTEQ,
  LT_GT,
  LT_GTEQ,
  LT_SHL,
  LT_SHR,
  LT_POINT
  // LT_
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
  std::string& get_str();
  int get_row();
  int get_col();
  int get_type();
  std::string get_type_str();
  std::string get_value();
  bool is_leteral();
  bool is_identificator();
  bool is_separator();
  bool is_close_separator();
  void print();
  bool operator==(const LexemeType&) const;
  bool operator!=(const LexemeType&) const;
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

  class Base_lexeme_error{
  public:
    Base_lexeme_error(){};
    Base_lexeme_error(Lexeme l, std::string s): lex(l), str(s){};
    virtual void print(){
      std::cout << str << std::endl;
      lex.print();
    };
    virtual std::string get_error_str(){
      std::stringstream ss;
      ss << str << "\n"
      << lex.get_row() + 1 << " " << lex.get_col() + 1 << " "
      << lex.get_type_str() << " " << lex.get_value() << "\n";
      return ss.str();
    };
    Lexeme lex;
  private:
    std::string str;
  };

  class Unknown_lexeme: public Base_lexeme_error{
  public:
    Unknown_lexeme(){};
    Unknown_lexeme(Lexeme l): Base_lexeme_error(l, "Unknown lexeme"){};
    Lexeme lex;
  };

  class Illegal_expression: public Base_lexeme_error{
  public:
    Illegal_expression(){};
    Illegal_expression(Lexeme l): Base_lexeme_error(l, "Illegal expression"){};
    Lexeme lex;
  };

  class ClosingParenthesisNotFound: public Base_lexeme_error{
  public:
    ClosingParenthesisNotFound(){};
    ClosingParenthesisNotFound(Lexeme l): Base_lexeme_error(l, "Closing parenthesis not found"){};
    // Lexeme lex;
  };

  class TypeError: public Base_lexeme_error{
  public:
    TypeError(){};
    TypeError(Lexeme l): Base_lexeme_error(l, "TypeError"){};
    // Lexeme lex;
  };

  class WrongTypeForBinaryOperator: public Base_lexeme_error{
  public:
    WrongTypeForBinaryOperator(){};
    WrongTypeForBinaryOperator(Lexeme l): Base_lexeme_error(l, "Wrong Types"){};
    // Lexeme lex;
  };

  class SymbolError{
  public:
    SymbolError(){};
    SymbolError(std::string vn): var_name(vn){};
    void print(){
      std::cout << var_name << " already in the symbol table" << std::endl;
    };
    std::string var_name;
  };

  class NoneInSymbolTable{
  public:
    NoneInSymbolTable(){};
    NoneInSymbolTable(std::string _name): name(_name){};
    std::string name;
  };

  class WrongType{
  public:
    WrongType(){}
    WrongType(Lexeme lex): lexeme(lex){};
    Lexeme lexeme;
  };

  class NoLValueInAssigment{
  public:
    NoLValueInAssigment(){}
    NoLValueInAssigment(Lexeme lex): lexeme(lex){};
    Lexeme lexeme;
  };

  class WrongTypeForAssigmentOperator{
  public:
    WrongTypeForAssigmentOperator(){}
    WrongTypeForAssigmentOperator(Lexeme lex): lexeme(lex){};
    Lexeme lexeme;
  };

  class WrongBinaryOperator{
  public:
    WrongBinaryOperator(){}
    WrongBinaryOperator(Lexeme lex): lexeme(lex){};
    Lexeme lexeme;
  };

  class WrongTypeOfExprInIf{
  public:
    WrongTypeOfExprInIf(){}
    WrongTypeOfExprInIf(Lexeme lex): lexeme(lex){};
    Lexeme lexeme;
  };

  class WrongTypeOfExprInWhile{
  public:
    WrongTypeOfExprInWhile(){}
    WrongTypeOfExprInWhile(Lexeme lex): lexeme(lex){};
    Lexeme lexeme;
  };

  class WrongTypeOfExprInFor{
  public:
    WrongTypeOfExprInFor(){}
    WrongTypeOfExprInFor(Lexeme lex): lexeme(lex){};
    Lexeme lexeme;
  };

};
