#include <syntaxis_analyser.h>
#include <map>

using namespace std;

map<string, int> operation_unary_postfix = {
  {"()", 1},
  {"[]", 1},
  {"++", 1},
  {"--", 1}
};
map<string, int> operation_unary_prefix = {
  {"++", 2},
  {"--", 2},
  {"+", 2},
  {"-", 2},
  {"!", 2}
};
map<string, int> operation_binary_left = {
  {".", 1},
  {"*", 5},
  {"/", 5},
  {"%", 5},
  {"+", 6},
  {"-", 6},
  {"<<", 7},
  {">>", 7},
  {"<", 8},
  {"<=", 8},
  {">", 8},
  {">=", 8},
  {"==", 9},
  {"!=", 9},
  {"&", 10},
  {"^", 11},
  {"|", 12},
  {"&&", 13},
  {"||", 14},
  {",", 16}
};
map<string, int> operation_binary_right = {
  {"=", 15},
  {"+=", 15},
  {"-=", 15},
  {"*=", 15},
  {"/=", 15},
  {"%=", 15},
  {"&=", 15},
  {"|=", 15},
  {"^=", 15}
};

bool is_terminal(Lexeme l){
  switch(l.get_type()){
    case TYPE_VAR:
    case TYPE_IDENTIFICATE:
    case TYPE_ARRAY:
    case TYPE_INT:
    case TYPE_FLOAT:
    case TYPE_STRING:
      return true;
  }
  return false;
}

PNocle SyntaxisAnalyser::get_tree(){
  searcher.next();
  return parse_expr(0);
}

PNocle SyntaxisAnalyser::parse_expr(int prioryty){
  if (priority > 16){
    return terminal_found();
  }
  PNocle temp = parse_expr(priority + 1);
  while(check_priority(temp, )){
    Lexeme next = searcher.next();
    PNocle right = parse_expr(priority + 1);
    temp = PNocle(new )
  }
}

PNocle SyntaxisAnalyser::terminal_found(PNocle& tree){
  Lexeme cur = searcher.get_current();
  if (tree == NULL){
    return PNocle(new NocleTerminate(cur));
  }
  switch(tree->get_type()){
    case NT_BINARY_RIGHT:
      PNocleBinaryRight nbr(tree);
      nbr->set_child2(PNocle(new NocleTerminate(cur)));
    case NT_UNARY_PREFIX:
      PNocleUnaryPrefix nu(tree);
      nu->set_child(PNocle(new NocleTerminate(cur)));
    default:
      throw 11;
  }
  return tree;

}

PNocle SyntaxisAnalyser::separator_found(PNocle& tree){
  Lexeme cur = searcher.get_current();
  if (cur.get_str() == "("){

  }
}

PNocle SyntaxisAnalyser::unary_operator_prefix(PNocle& tree){

}

PNocle SyntaxisAnalyser::not_terminal_found(PNocle& tree){

}
