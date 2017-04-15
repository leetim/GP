#include <syntax_tree.h>
#include <iostream>
#include <map>

using namespace std;
using namespace Symbol;

map <BaseSymbolType, string> type_name;

////////////////////////////////////////////////////////////////////////////////
//Nocle

void Nocle::set_lexeme(Lexeme lex){
  lexeme = lex;
}

Lexeme Nocle::get_lexeme(){
  return lexeme;
}

PType Nocle::get_result_type(PSymbolTable t){
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

PType NocleTerminate::get_result_type(PSymbolTable t){
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
//NocleInteger

PType NocleInteger::get_result_type(PSymbolTable t){
    return t->get_type_from_str("Int");
}

////////////////////////////////////////////////////////////////////////////////
//NocleFloat

PType NocleFloat::get_result_type(PSymbolTable t){
    return t->get_type_from_str("Float");
}

////////////////////////////////////////////////////////////////////////////////
//NocleString

PType NocleString::get_result_type(PSymbolTable t){
    return t->get_type_from_str("String");
}

////////////////////////////////////////////////////////////////////////////////
//NocleVariable

Symbol::PType NocleVariable::get_result_type(PSymbolTable t){
  return t->get_symbol(get_lexeme().get_str())->get_type();
}

////////////////////////////////////////////////////////////////////////////////
//NocleTypeName

Symbol::PType NocleTypeName::get_result_type(PSymbolTable t){
  return t->get_type_from_str(get_lexeme().get_str());
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

Symbol::PType NocleUnary::get_result_type(PSymbolTable t){
  return get_child()->get_result_type(t);
}

////////////////////////////////////////////////////////////////////////////////
//NocleUnaryCast

// NocleUnaryCast(Lexeme lex, PNocle ch = NULL){
//   set_child(ch);
//   cast_to =
// }

NocleUnaryCast::NocleUnaryCast(Symbol::PType t, PNocle ch){
  set_child(ch);
  cast_to = t;
  Lexeme l = ch->get_lexeme();
  l.get_str() = "cast_to_" + t->get_type_name();
  set_lexeme(l);
}


Symbol::PType NocleUnaryCast::get_result_type(PSymbolTable t){
  return cast_to;
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

bool NocleBinary::check_child_type(){
  return true;
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

PType NocleBinary::get_result_type(PSymbolTable t){
  PType tl = get_child1()->get_result_type(t);
  PType tr = get_child2()->get_result_type(t);
  if (!check_child_type(tl->get_base_type())){
    throw Errors::WrongTypeForBinaryOperator(get_lexeme());
  }
  if (tl->compare(tr)){
    return tl;
  }
  else{
    if (tl->is_casted_to(tr)){
      set_child1(PNocle(new NocleUnaryCast(tr, get_child1())));
      return tl;
    }
    throw Errors::WrongTypeForBinaryOperator(get_lexeme());
  }
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryArithmetic

bool NocleBinaryArithmetic::check_child_type(BaseSymbolType bst){
  return bst == BST_INTEGER || bst == BST_FLOAT;
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryBitwice

bool NocleBinaryBitwice::check_child_type(BaseSymbolType bst){
  return bst == BST_INTEGER;
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryLogic

bool NocleBinaryLogic::check_child_type(BaseSymbolType bst){
  return bst == BST_BOOL;
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryOther

PType NocleBinaryOther::get_result_type(PSymbolTable t){
  PType tl = get_child1()->get_result_type(t);
  PType tr = get_child2()->get_result_type(t);
  if (!check_child_type(tl->get_base_type())){
    throw Errors::WrongTypeForBinaryOperator(get_lexeme());
  }
  if (tl->compare(tr)){
    return result_type;
  }
  else{
    if (tr->is_casted_to(tl)){
      set_child2(PNocle(new NocleUnaryCast(tr, get_child2())));
      return result_type;
    }
    throw Errors::WrongTypeForBinaryOperator(get_lexeme());
  }
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryRange

NocleBinaryRange::NocleBinaryRange(Lexeme lex, PNocle ch1, PNocle ch2){
  set_lexeme(lex);
  set_child1(ch1);
  set_child2(ch2);
  result_type = get_base_type_from_str("Range");
}

bool NocleBinaryRange::check_child_type(BaseSymbolType bst){
  return bst == BST_INTEGER;
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryPoint

bool NocleBinaryPoint::check_child_type(BaseSymbolType bst){
  return bst == BST_RECORD;
}

////////////////////////////////////////////////////////////////////////////////
//NocleArrayElement


PType NocleArrayElement::get_result_type(PSymbolTable t){
  PType tl = get_child1()->get_result_type(t);
  PType tr = get_child2()->get_result_type(t);
  if (!check_child_type(tl->get_base_type())){
    throw Errors::WrongTypeForBinaryOperator(get_lexeme());
  }
  if (tl->compare(tr)){
    return result_type;
  }
  else{
    if (tr->is_casted_to(tl)){
      set_child2(PNocle(new NocleUnaryCast(tr, get_child2())));
      return result_type;
    }
    throw Errors::WrongTypeForBinaryOperator(get_lexeme());
  }
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

PType NocleMulty::get_result_type(PSymbolTable t){
  return PType(new Void());;
}


////////////////////////////////////////////////////////////////////////////////
//

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryRight

PType NocleBinaryRight::get_result_type(PSymbolTable t){
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

void NocleBlock::print_table(){
  table->print();
}

////////////////////////////////////////////////////////////////////
//Variable


Variable::Variable(std::string name, PType t){
  set_name(name);
  type = t;
}

Variable::Variable(std::string name, PType t, PNocle init){
  set_name(name);
  // cout << t->get_name() << endl;
  type = t;
  if (init){
    initialization = init;
  }
  else{
    throw 1;
  }
}

string Variable::get_type_name(){
  // stringstream ss;
  // cout << type->get_name() << endl;
  return type->get_name();
}

PType Variable::get_type(){
  return type;
}

////////////////////////////////////////////////////////////////////////////////
//Symbol::Procedure

Procedure::Procedure(std::string name, PSymbolTable st){
  this->set_name(name);
  stable = st;
}

void Procedure::set_block(PNocle b){
  block = b;
}
