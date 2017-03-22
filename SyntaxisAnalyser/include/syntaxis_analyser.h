#pragma once

#include <lexicalSearch.h>
#include <syntax_tree.h>
#include <map>

class SyntaxisAnalyser{
public:
  SyntaxisAnalyser(){};
  SyntaxisAnalyser(std::string file_name): searcher(file_name){};
  PNocle get_tree();
private:
  PNocle parse_expr(int prioryty = 1);
  PNocle parse_factor();
  PNocle parse_identificator();
  bool require_lexeme(std::string t);
  bool check_priority(Lexeme lex, int priority);
  static int max_priority;
  static std::map<std::string, int> operation_unary_postfix;
  static std::map<std::string, int> operation_unary_prefix;
  static std::map<std::string, int> operation_binary_left;
  static std::map<std::string, int> operation_binary_right;
  PNocle root;
  Searcher searcher;
};
