#include <symbol_table.h>

////////////////////////////////////////////////////////////////////////////////
//SymbolTable

SymbolTable::SymbolTable(){
  high_lvl = PSymbolTable();
}

SymbolTable::SymbolTable(PSymbolTable hl){
  high_lvl = hl;
}

void SymbolTable::add_symbol(Symbol::PBase arg){
  vect_symbs.push_back(arg);
  map_symbs[arg->get_name()] = arg;
}

Symbol::PBase SymbolTable::operator[](std::string ind){
  return map_symbs[ind];
}

Symbol::PBase SymbolTable::operator[](int ind){
  return vect_symbs[ind];
}

unsigned int SymbolTable::get_count(){
  return vect_symbs.size();
}
