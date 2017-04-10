#include <symbol.h>

using namespace std;
using namespace Symbol;


////////////////////////////////////////////////////////////////////
//Base

string Base::get_name(){
  return name;
}

void Base::set_name(string n){
  name = n;
}

PType Base::get_type(){
  return PType(new Void());
}


////////////////////////////////////////////////////////////////////
//Type

Type::Type(string s){
  set_name(s);
}

Type::Type(string s, BaseSymbolType bst){
  base_type = bst;
  set_name(s);
}

BaseSymbolType Type::get_base_type(){
  return base_type;
}

bool Type::compare(PType t){
  return t->get_base_type() == base_type;
};

////////////////////////////////////////////////////////////////////
//Reference

bool Reference::compare(PType t){
  if (t->get_base_type() == this->get_base_type()){
    shared_ptr<Reference> temp = static_pointer_cast<Reference>(t);
    return temp->type->compare(this->type);
  }
  else{
    return false;
  }
}

////////////////////////////////////////////////////////////////////
//Array

bool Array::compare(PType t){
  if (t->get_base_type() == this->get_base_type()){
    shared_ptr<Array> temp = static_pointer_cast<Array>(t);
    return temp->type->compare(this->type);
  }
  else{
    return false;
  }
}

////////////////////////////////////////////////////////////////////
//Alias

bool Alias::compare(PType t){
  if (t->get_base_type() == this->get_base_type()){
    shared_ptr<Alias> temp = static_pointer_cast<Alias>(t);
    return temp->type->compare(this->type);
  }
  else{
    return false;
  }
}

////////////////////////////////////////////////////////////////////
//Variable

Variable::Variable(string n, PType t){
  type = t;
  set_name(n);
}

PType Variable::get_type(){
  return type;
}
