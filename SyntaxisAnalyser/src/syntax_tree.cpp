#include <syntax_tree.h>
#include <iostream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//Nocle

void Nocle::set_lexeme(Lexeme lex){
  lexeme = lex;
}

Lexeme Nocle::get_lexeme(){
  return lexeme;
}

NocleType Nocle::get_type(){
  return NT_BASE;
}

string Nocle::get_str(){
  stringstream ss;
  get_str(ss, 0);
  return ss.str();
}

void Nocle::get_str(stringstream& ss, int depth){
  cout << "NNYA" << endl;
  // return "";
}


////////////////////////////////////////////////////////////////////////////////
//NocleTerminate

NocleType NocleTerminate::get_type(){
  return NT_TERMINATE;
}

void NocleTerminate::get_str(stringstream& ss, int depth){
  // cout << "NYA" << endl;
  ss << string(depth*8, '_') << get_lexeme().get_value() << "\n";
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
  ss << string(depth*8, '_') << get_lexeme().get_value() << "\n";
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
  child1->get_str(ss, depth + 1);
  ss << string(depth*8, '_') << get_lexeme().get_value() << "\n";
  child2->get_str(ss, depth + 1);
  // return get_lexeme().get_value();
}

////////////////////////////////////////////////////////////////////////////////
//NocleExpression

NocleType NocleExpression::get_type(){
  return NT_EXPR;
}

////////////////////////////////////////////////////////////////////////////////
//NocleUnaryPrefix

NocleType NocleUnaryPrefix::get_type(){
  return NT_UNARY_PREFIX;
}

////////////////////////////////////////////////////////////////////////////////
//NocleUnaryPostfix

NocleType NocleUnaryPostfix::get_type(){
  return NT_UNARY_POSTFIX;
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryLeft

NocleType NocleBinaryLeft::get_type(){
  return NT_BINARY_LEFT;
}

////////////////////////////////////////////////////////////////////////////////
//NocleBinaryRight

NocleType NocleBinaryRight::get_type(){
  return NT_BINARY_RIGHT;
}
