#include <symbol_table.h>
#include <iostream>

using namespace Symbol;
using namespace std;

////////////////////////////////////////////////////////////////////////////////
//SymbolTable

map<string, PType> base_types = {
  {"Void", PType(new Void)},
  {"Int", PType(new Integer)},
  {"Float", PType(new Float)},
  {"String", PType(new String)},
  {"Bool", PType(new Bool)},
  {"Range", PType(new Range)}
};

SymbolTable::SymbolTable(){
  high_lvl = PSymbolTable();
}

SymbolTable::SymbolTable(PSymbolTable hl){
  high_lvl = hl;
}

void SymbolTable::add_symbol(Symbol::PBase arg){
  // cout << arg->get_name() << endl;
  if (map_symbs[arg->get_name()]){
    // cout << arg->get_name() << endl;
    throw Errors::SymbolError(arg->get_name());
  }
  vect_symbs.push_back(arg);
  map_symbs[arg->get_name()] = arg;
}

Symbol::PBase SymbolTable::get_symbol(std::string ind){
  PBase temp = map_symbs[ind];
  if (temp == NULL && high_lvl != NULL){
    return (*high_lvl)[ind];
  }
  else{
    throw Errors::NoneInSymbolTable(ind);
  }
  return map_symbs[ind];
}

PBase SymbolTable::operator[](string ind){
  return this->get_symbol(ind);
}

PBase SymbolTable::operator[](int ind){
  return this->get_symbol(ind);
}

Symbol::PType SymbolTable::get_type_from_str(string str){
  if (base_types[str] == NULL){
    PBase symb = this->get_symbol(str);
    // if (symb.get_base_type())
    return static_pointer_cast<Type>(symb);
  }
  else{
    return base_types[str];
  }
}

Symbol::PBase SymbolTable::get_symbol(int ind){
  return vect_symbs[ind];
}

unsigned int SymbolTable::get_count(){
  return vect_symbs.size();
}

void SymbolTable::print(int lvl){
  cout << lvl << endl;
  for (auto i: vect_symbs){
    cout << '\t' << i->get_name() << endl;
  }
  if (high_lvl){
    high_lvl->print(lvl+1);
  }

}
