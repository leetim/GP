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
  // PNocle separator_found(PNocle& tree);
  // PNocle not_terminal_found(PNocle& tree);
  // PNocle terminal_found(PNocle& tree);
  // PNocle unary_operator_prefix(PNocle& tree);
  static int max_priority;
  static std::map<std::string, int> operation_unary_postfix;
  static std::map<std::string, int> operation_unary_prefix;
  static std::map<std::string, int> operation_binary_left;
  static std::map<std::string, int> operation_binary_right;
  PNocle root;
  Searcher searcher;
};
