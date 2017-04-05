#pragma once

#include <symbol.h>
#include <vector>
#include <map>

class SymbolTable;
typedef std::shared_ptr<SymbolTable> PSymbolTable;

class SymbolTable{
public:
  SymbolTable();
  SymbolTable(PSymbolTable hl);
  void add_symbol(Symbol::PBase arg);
  Symbol::PBase operator[](std::string);
  Symbol::PBase operator[](int);
  Symbol::PBase get_symbol(std::string);
  Symbol::PBase get_symbol(int);
  Symbol::PType get_type_from_str(std::string);
  void print(int lvl = 0);
  unsigned int get_count();
  // static Symbol::PSymbolTable
private:
  PSymbolTable high_lvl;
  std::vector<Symbol::PBase> vect_symbs;
  std::map<std::string, Symbol::PBase> map_symbs;
};
