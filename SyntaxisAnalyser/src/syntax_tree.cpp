#include <syntax_tree.h>
#include <iostream>

using namespace std;
using namespace Symbol;

////////////////////////////////////////////////////////////////////////////////
//Nocle

void Nocle::set_lexeme(Lexeme lex){
  lexeme = lex;
}

Lexeme Nocle::get_lexeme(){
  return lexeme;
}

PType Nocle::get_type(PSymbolTable t){
  return PType(new Void());
}

string Nocle::get_str(){
  stringstream ss;
  get_str(ss, 0);
  // ss << "\n";
  return ss.str();
}

void Nocle::get_str(stringstream& ss, int depth){
  ss << "";
  // cout << "NNYA" << endl;
  // return "";
}


bool Nocle::check_lvalue(PSymbolTable){
  return false;
}

bool Nocle::check_types(PSymbolTable){
  return true;
}

// Symbol::PType get_type(){
//   return PType(new Symbol::Integer());
// }


////////////////////////////////////////////////////////////////////////////////
//NocleTerminate

PType NocleTerminate::get_type(PSymbolTable t){
  return t->get_symbol(get_lexeme().get_str())->get_type();
}

void NocleTerminate::get_str(stringstream& ss, int depth){
  // cout << "NYA" << endl;
  if (depth == 0){
    ss << get_lexeme().get_value() << "\n";
  }
  else{
    ss << string((depth - 1)*4, ' ') << "|___" << get_lexeme().get_value() << "\n";
  }
  // return get_lexeme().get_value();
}

////////////////////////////////////////////////////////////////////////////////
//NocleUnary

void NocleUnary::set_child(PNocle ch){
  child = ch;
}

PNocle NocleUnary::get_child(){
  return child;
}

void NocleUnary::get_str(stringstream& ss, int depth){
  if (depth == 0){
    ss << get_lexeme().get_value() << "\n";
  }
  else{
    ss << string((depth - 1)*4, ' ') << "|___" << get_lexeme().get_value() << "\n";
  }
  child->get_str(ss, depth + 1);
  // return get_lexeme().get_value();
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinary

void NocleBinary::set_child1(PNocle ch1){
  child1 = ch1;
}

PNocle NocleBinary::get_child1(){
  return child1;
}

void NocleBinary::set_child2(PNocle ch2){
  child2 = ch2;
}

PNocle NocleBinary::get_child2(){
  return child2;
}

void NocleBinary::get_str(stringstream& ss, int depth){
  if (depth == 0){
    ss << get_lexeme().get_value() << "\n";
  }
  else{
    ss << string((depth - 1)*4, ' ') << "|___" << get_lexeme().get_value() << "\n";
  }
  child1->get_str(ss, depth + 1);
  child2->get_str(ss, depth + 1);
  // return get_lexeme().get_value();
}

////////////////////////////////////////////////////////////////////////////////
//NocleMulty

void NocleMulty::add_child(PNocle ch){
  children.push_back(ch);
}

PNocle NocleMulty::get_child(int ind){
  return children.at(ind);
}

PNocle NocleMulty::operator[](int ind){
  return children.at(ind);
}

void NocleMulty::get_str(std::stringstream& ss, int depth){
  if (depth == 0){
    ss << get_lexeme().get_value() << "\n";
  }
  else{
    ss << string((depth - 1)*4, ' ') << "|___" << get_lexeme().get_value() << "\n";
  }
  for (auto i = children.begin(); i != children.end(); i++){
    (*i)->get_str(ss, depth + 1);
  }
}

PType NocleMulty::get_type(PSymbolTable t){
  return PType(new Void());;
}

////////////////////////////////////////////////////////////////////////////////
//NocleUnaryPrefix

PType NocleUnaryPrefix::get_type(PSymbolTable t){
  return get_child()->get_type(t);
}

////////////////////////////////////////////////////////////////////////////////
//NocleUnaryPostfix

PType NocleUnaryPostfix::get_type(PSymbolTable t){
  return get_child()->get_type(t);
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryLeft

PType NocleBinaryLeft::get_type(PSymbolTable t){
  PType tl = get_child1()->get_type(t);
  PType tr = get_child2()->get_type(t);
  if (tl->compare(tr)){
    return tl;
  }
  else{
    //try cast
    // throw Errors::;
  }
  return PType(new Void());
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryRight

PType NocleBinaryRight::get_type(PSymbolTable t){
  return PType(new Void());
}

////////////////////////////////////////////////////////////////////////////////
//NocleBlock

bool NocleBlock::check_types(PSymbolTable t){
  for (auto i: children){
    if (!i->check_types(t)){
      return false;
      // throw Errors::TypeError();
    }
  }
  return true;
}

////////////////////////////////////////////////////////////////////////////////
//Symbol::Procedure

Procedure::Procedure(std::string name, PSymbolTable st, PNocle b){
  this->set_name(name);
  stable = st;
  block = b;
}
