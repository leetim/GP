#include <symbol.h>
#include <map>

using namespace std;
using namespace Symbol;

map<BaseSymbolType, string> type_names = {
  {BST_NONE, "NONE"},
  {BST_VOID, "VOID"},
  {BST_INTEGER, "INTEGER"},
  {BST_FLOAT, "FLOAT"},
  {BST_STRING, "STRING"},
  {BST_BOOL, "BOOL"},
  {BST_RANGE, "RANGE"},
  {BST_RECORD, "RECORD"},
  {BST_REFERENCE, "REFERENCE"},
  {BST_ARRAY, "ARRAY"},
  {BST_ALIAS, "ALIAS"},
  {BST_CLASS, "CLASS"}
};


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

string Base::get_type_name(){
  return "";
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

PType Type::get_type(){
  return PType(new Class());
}

bool Type::is_casted_to(PType t){
  BaseSymbolType bst = t->get_base_type();
  if (bst == base_type){
    return true;
  }
  switch (base_type){
    case BST_INTEGER:
      return bst == BST_FLOAT || bst == BST_STRING;
    case BST_FLOAT:
      return bst == BST_STRING;
    default:
      return false;
  }
}

bool Type::compare(PType t){
  return t->get_base_type() == base_type;
};

string Type::get_type_name(){
  // cout << type_names[get_base_type()] << endl;
  return type_names[BST_CLASS];
}

////////////////////////////////////////////////////////////////////
//Reference

// bool Reference::compare(PType t){
//   if (t->get_base_type() == this->get_base_type()){
//     shared_ptr<Reference> temp = static_pointer_cast<Reference>(t);
//     return temp->type->compare(this->type);
//   }
//   else{
//     return false;
//   }
// }

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

string Array::get_name(){
  stringstream ss;
  // ss << type->get_name() << "[" << count << "]";
  return ss.str();
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
