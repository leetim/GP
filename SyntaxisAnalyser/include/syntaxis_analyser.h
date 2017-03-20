#pragma once

#include <lexicalSearch.h>
#include <syntax_tree.h>

class SyntaxisAnalyser{
public:
  SyntaxisAnalyser(){};
  SyntaxisAnalyser(std::string file_name): searcher(file_name){};
  PNocle get_tree();
private:
  PNocle parse_expr(int prioryty);
  PNocle separator_found(PNocle& tree);
  PNocle not_terminal_found(PNocle& tree);
  PNocle terminal_found(PNocle& tree);
  PNocle unary_operator_prefix(PNocle& tree);
  PNocle root;
  Searcher searcher;
};
