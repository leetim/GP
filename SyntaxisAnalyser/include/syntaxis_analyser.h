#pragma once

#include <lexicalSearch.h>
#include <syntax_tree.h>
#include <map>
#include <iostream>

class SyntaxisAnalyser{
public:
  SyntaxisAnalyser(){};
  SyntaxisAnalyser(std::string file_name): searcher(file_name){};
  PNocle get_tree();
  PNocle parse_program();
private:
  PNocle parse_expr(int prioryty = 1);
  PNocle parse_factor();
  PNocle parse_identificator();
  PNocle parse_function_call(PNocle ident, LexemeType finish);

  PNocle parse_statment(PSymbolTable t = NULL);
  PNocle parse_simple_statment(PSymbolTable t = NULL);
  PNocle parse_block(PSymbolTable t, LexemeType end = LT_END);
  PNocle parse_while_cycle(PSymbolTable t = NULL);
  PNocle parse_for_cycle(PSymbolTable t = NULL);
  PNocle parse_if(PSymbolTable t = NULL);
  PNocle parse_def_function(PSymbolTable t = NULL);
  PNocle parse_type(PSymbolTable t = NULL);

  PNocle parse_def_variable(PSymbolTable t = NULL);
  bool require_lexeme(LexemeType lt);
  bool check_priority(Lexeme lex, int priority);
  static int max_priority;
  static std::map<std::string, int> operation_unary_postfix;
  static std::map<std::string, int> operation_unary_prefix;
  static std::map<std::string, int> operation_binary_left;
  static std::map<std::string, int> operation_binary_right;
  PNocle root;
  Searcher searcher;
};
