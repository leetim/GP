#include <syntax_tree.h>
#include <iostream>
#include <map>

using namespace std;
using namespace Symbol;

map <BaseSymbolType, string> type_name;

////////////////////////////////////////////////////////////////////////////////
//functions

template<typename EXCEPTION>
void try_cast(PType to, PType from, PNocle& node, Lexeme lex){
  cout << from->get_name() << "->"
  << to->get_name() << " " << endl;
  lex.print();
  if (!from->compare(to)){
    if (from->is_casted_to(to)){
      node = PNocle(
        new NocleUnaryCast(to, node)
      );
    }
    else{
      throw EXCEPTION(lex);
    }
  }
}

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
  // cout << "!lvalue_checked" << endl;
  return false;
}

bool Nocle::check_types(PSymbolTable){
  // cout << "\n\n\nnode check\n\n\n" << endl;
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
//NocleBool

PType NocleBool::get_result_type(PSymbolTable t){
    return t->get_type_from_str("Bool");
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
  // cout << "\t\t\t\tAdded variable" << endl;
  return t->get_symbol(get_lexeme().get_str())->get_type();
}

bool NocleVariable::check_lvalue(PSymbolTable t){
  // cout << "lvalue_checked" << endl;
  return true;
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

NocleUnaryCast::NocleUnaryCast(Lexeme lex, PNocle ch){
  set_child(ch);
  lex.get_str() = "cast_to_" + lex.get_str();
  set_lexeme(lex);
  cast_to = NULL;
}

NocleUnaryCast::NocleUnaryCast(Symbol::PType t, PNocle ch){
  set_child(ch);
  cast_to = t;
  Lexeme l = ch->get_lexeme();;
  l.get_str() = "cast_to_" + t->get_name();
  set_lexeme(l);
}


Symbol::PType NocleUnaryCast::get_result_type(PSymbolTable t){
  if (!cast_to){
    cast_to = t->get_type_from_str(get_lexeme().get_str());
    get_lexeme().get_str() = "cast_to_" + get_lexeme().get_str();
  }
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

bool NocleBinary::check_child_type(BaseSymbolType bst1, BaseSymbolType bst2){
  return true;
}

bool NocleBinary::check_types(PSymbolTable t){
  get_result_type(t);
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
  if (!check_child_type(tl->get_base_type(), tr->get_base_type())){
    throw Errors::WrongTypeForBinaryOperator(get_lexeme());
  }
  if (tl->compare(tr)){
    return tl;
  }
  else{
    cout << "\t" << tl->get_name() << endl;
    cout << "\t" << tr->get_name() << endl;
    if (tr->is_casted_to(tl)){
      set_child2(PNocle(new NocleUnaryCast(tl, get_child2())));
      return tl;
    }
    if (tl->is_casted_to(tr)){
      set_child1(PNocle(new NocleUnaryCast(tr, get_child1())));
      return tr;
    }
    throw Errors::WrongTypeForBinaryOperator(get_lexeme());
  }
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryArithmetic

bool NocleBinaryArithmetic::check_child_type(BaseSymbolType bst1, BaseSymbolType bst2){
  return (bst1 == BST_INTEGER || bst1 == BST_FLOAT) &&
    (bst2 == BST_INTEGER || bst2 == BST_FLOAT);
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryBitwice

bool NocleBinaryBitwice::check_child_type(BaseSymbolType bst1, BaseSymbolType bst2){
  return (bst1 == BST_INTEGER) && (bst2 == BST_INTEGER);
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryLogic

bool NocleBinaryLogic::check_child_type(BaseSymbolType bst1, BaseSymbolType bst2){
  return (bst1 == BST_BOOL) && (bst2 == BST_BOOL);
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryOther

Symbol::PType NocleBinaryOther::get_wanted_result_type(PSymbolTable t){
  return result_type;
}

PType NocleBinaryOther::get_result_type(PSymbolTable t){
  PType tl = get_child1()->get_result_type(t);
  PType tr = get_child2()->get_result_type(t);
  if (!check_child_type(tl->get_base_type(), tr->get_base_type())){
    throw Errors::WrongTypeForBinaryOperator(get_lexeme());
  }
  if (tl->compare(tr)){
    return get_wanted_result_type(t);
  }
  else{
    if (tr->is_casted_to(tl)){
      set_child2(PNocle(new NocleUnaryCast(tl, get_child2())));
      return get_wanted_result_type(t);
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

bool NocleBinaryRange::check_child_type(BaseSymbolType bst1, BaseSymbolType bst2){
  return (bst1 == BST_INTEGER) && (bst2 == BST_INTEGER);
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryPoint

bool NocleBinaryPoint::check_lvalue(PSymbolTable t){
  cout << "check_lvalue" <<endl;
  return get_child2()->check_lvalue(t);
}

PType NocleBinaryPoint::get_result_type(PSymbolTable t){
  if (get_child1()->get_lexeme().get_type() != LT_VARIABLE){
    throw Errors::WrongTypeForBinaryOperator(get_lexeme());
  }
  if (get_child1()->get_lexeme().get_type() != LT_VARIABLE){
    throw Errors::WrongTypeForBinaryOperator(get_lexeme());
  }
  shared_ptr<Record> tl = static_pointer_cast<Record>(
    get_child1()->get_result_type(t)
  );
  PType tr = get_child2()->get_result_type(tl->get_table());
  cout << "check_point " << tl->get_name() << endl;
  cout << "check_point " << BST_RECORD << endl;
  if (check_child_type(tl->get_base_type(), tr->get_base_type())){
    return tr;
  }
  throw Errors::WrongTypeForBinaryOperator(get_lexeme());
}

bool NocleBinaryPoint::check_child_type(BaseSymbolType bst1, BaseSymbolType bst2){
  return bst1 == BST_RECORD;
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryCompare

bool NocleBinaryCompare::check_child_type(BaseSymbolType bst1, BaseSymbolType bst2){
  return bst1 == BST_INTEGER || bst2 == BST_INTEGER ||
         bst1 == BST_FLOAT || bst2 == BST_FLOAT ||
         bst1 == BST_STRING || bst2 == BST_STRING;
}

PType NocleBinaryCompare::get_wanted_result_type(PSymbolTable t){
  if (!result_type){
    result_type = t->get_type_from_str("Bool");
  }
  return result_type;
}

////////////////////////////////////////////////////////////////////////////////
//NocleArrayElement

PType NocleArrayElement::get_wanted_result_type(PSymbolTable t){
  if (!result_type){
    PArray tl = static_pointer_cast<Array>(
      get_child1()->get_result_type(t)
    );
    result_type = tl->get_element_type();
  }
  return result_type;
  // PType tr = get_child2()->get_result_type(t);
}

bool NocleArrayElement::check_child_type(BaseSymbolType bst1, BaseSymbolType bst2){
  return bst1 == BST_ARRAY && bst2 == BST_INTEGER;
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryAssigment

bool NocleBinaryAssigment::check_types(PSymbolTable t){
  get_child1()->get_lexeme().print();
  if (!get_child1()->check_lvalue(t)){
    throw Errors::NoLValueInAssigment(get_lexeme());
  }
  PType left = get_child1()->get_result_type(t);
  PType right = get_child2()->get_result_type(t);
  if (!left->compare(right)){
    if (right->is_casted_to(left)){
      set_child2(PNocle(new NocleUnaryCast(left, get_child2())));
    }
    else{
      throw Errors::WrongTypeForAssigmentOperator(get_lexeme());
    }
  }
  return true;
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
//NocleFunction

PType NocleFunction::get_result_type(PSymbolTable t){
  check_types(t);
  PProcedure p = static_pointer_cast<Procedure>(
    t->get_symbol(get_child(0)->get_lexeme().get_str())
  );
  return p->get_return_type();
}

bool NocleFunction::check_types(PSymbolTable t){
  cout << "func call check" << endl;
  cout << "\t" << children[0]->get_lexeme().get_str() << endl;

  PSymbolTable func_table = static_pointer_cast<Procedure>(
    t->get_symbol(children[0]->get_lexeme().get_str())
  )->get_table();
  func_table->print();
  for (unsigned int i = 0; i < func_table->get_count()-1; i++){
    PType need = static_pointer_cast<Type>(
      func_table->get_symbol(i)->get_type()
    );
    PType have = static_pointer_cast<Type>(
      children[i+1]->get_result_type(t)
    );
    cout << "\tType: " << need->get_name() << endl;
    need->compare(have);
    cout << "\tType: " << have->get_name() << endl;
    if (!(need->compare(have))){
      if (have->is_casted_to(need)){
        children[i] = PNocle(
          new NocleUnaryCast(need, children[i])
        );
      }
      else{
        throw Errors::WrongType(children[i]->get_lexeme());
      }
    }
    cout << "\t\tin if" << endl;
  }
  // int first = (!);
  // if ()
  return true;
}

////////////////////////////////////////////////////////////////////////////////
//NocleBlock

bool NocleBlock::check_types(PSymbolTable t){
  cout << "block check" << endl;
  for (auto i: children){
    if (!i->check_types(table)){
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
//NocleIf

bool NocleIf::check_types(PSymbolTable t){
  for (auto i: children){
    i->check_types(t);
  }
  PType log = children[0]->get_result_type(t);
  PType res = t->get_type_from_str("Bool");
  try_cast<Errors::WrongTypeOfExprInIf>(res, log, children[0], get_lexeme());
  return true;
}

////////////////////////////////////////////////////////////////////
//NocleWhile

bool NocleWhile::check_types(PSymbolTable t){
  PType log = get_child1()->get_result_type(t);
  PType res = t->get_type_from_str("Bool");
  try_cast<Errors::WrongTypeOfExprInWhile>(res, log, child1, get_lexeme());
  get_child2()->check_types(t);
  return true;
}

////////////////////////////////////////////////////////////////////
//NocleFor

bool NocleFor::check_types(PSymbolTable t){
  // PType iterator = children[0]->get_result_type(t);
  PType range = children[0]->get_result_type(t);
  // PType res0 = t->get_type_from_str("Int");
  PType res1 = t->get_type_from_str("Range");
  // try_cast<Errors::WrongTypeOfExprInFor>(res0, iterator, children[0], get_lexeme());
  try_cast<Errors::WrongTypeOfExprInFor>(res1, range, children[1], get_lexeme());
  children[1]->check_types(t);
  return true;
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
//Procedure

Procedure::Procedure(std::string name, PSymbolTable st){
  this->set_name(name);
  stable = st;
}

void Procedure::set_block(PNocle b){
  block = b;
}

PType Procedure::get_return_type(){
  PBase b = stable->get_symbol("$");
  if (!b){
    return stable->get_type_from_str("Bool");
  }
  return b->get_type();
}

PSymbolTable Procedure::get_table(){
  return stable;
}

////////////////////////////////////////////////////////////////////////////////
//Record

PBase Record::get_symbol(string name){
  return stable->get_symbol(name);
}

BaseSymbolType Record::get_base_type(){
  return BST_RECORD;
}

PSymbolTable Record::get_table(){
  return stable;
}

// void Procedure::set_block(PNocle b){
//   block = b;
// }
