#include <syntaxis_analyser.h>
#include <iostream>

using namespace std;

int SyntaxisAnalyser::max_priority = 13;
map<string, int> SyntaxisAnalyser::operation_unary_postfix = {
  {"()", 1},
  {"[]", 1},
  {"++", 1},
  {"--", 1}
};
map<string, int> SyntaxisAnalyser::operation_unary_prefix = {
  {"++", 2},
  {"--", 2},
  {"+", 2},
  {"-", 2},
  {"!", 2}
};
map<string, int> SyntaxisAnalyser::operation_binary_left = {
  {".", 13},
  {"*", 12},
  {"/", 11},
  {"%", 11},
  {"+", 10},
  {"-", 10},
  {"<<", 9},
  {">>", 9},
  {"<", 8},
  {"<=", 8},
  {">", 8},
  {">=", 8},
  {"==", 7},
  {"!=", 7},
  {"&", 6},
  {"^", 5},
  {"|", 4},
  {"&&", 3},
  {"||", 2}
  // {",", 1}
};
map<string, int> SyntaxisAnalyser::operation_binary_right = {
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

PNocle SyntaxisAnalyser::get_tree(){
  searcher.next();
  return parse_expr(1);
}

PNocle SyntaxisAnalyser::parse_expr(int priority){
  if (priority > 16){
    return parse_factor();
  }
  PNocle temp = parse_expr(priority + 1);
  Lexeme next = searcher.get_current();
  while(check_priority(next, priority)){
    searcher.next();
    PNocle right = parse_expr(priority + 1);
    temp = PNocle(new NocleBinaryLeft(next, temp, right));
    next = searcher.get_current();
  }
  return temp;
}

PNocle SyntaxisAnalyser::parse_factor(){
  Lexeme cur = searcher.get_current();
  // cout << cur.get_str() << endl;
  if (cur.is_identificator()){
    return parse_identificator();
  }

  Lexeme next = searcher.next();

  if (cur.is_leteral()){
    return PNocle(new NocleTerminate(cur));
  }
  if (operation_unary_prefix[cur.get_str()]){
    return PNocle(new NocleUnaryPrefix(cur, parse_factor()));
  }
  if (cur.get_str() == "("){
    PNocle temp = parse_expr(1);
    if (searcher.get_current().get_str() != ")"){
      throw Errors::Illegal_expression(cur); //Нужна адекватная ошибка
    }
    searcher.next();
    return temp;
  }
  // cout << next.get_str() << " " << cur.get_str() << endl;
  if (cur == LT_EOF){
    return PNocle(new Nocle(cur));
  }
  throw Errors::Illegal_expression(cur); //Нужна адекватная ошибка
  // return NULL;
}

PNocle SyntaxisAnalyser::parse_identificator(){
  Lexeme cur = searcher.get_current();
  Lexeme next = searcher.next();
  PNocle ident(new NocleTerminate(cur));
  // cout << "REQ " << cur.get_str() << endl;
  if (next.get_str() == "["){
    searcher.next();
    PNocle temp = parse_expr();
    require_lexeme("]");
    searcher.next();
    return PNocle(new NocleBinaryRight(next, ident, temp));
  }
  if (next.get_str() == "("){
    searcher.next();
    PNocle temp = parse_expr();
    require_lexeme(")");
    searcher.next();
    return PNocle(new NocleBinaryRight(next, ident, temp));
  }
  return ident;
}

bool SyntaxisAnalyser::require_lexeme(string t){
  if (searcher.get_current().get_value() == t)
    return true;
  throw Errors::ClosingParenthesisNotFound(searcher.get_current());
}


bool SyntaxisAnalyser::check_priority(Lexeme lex, int priority){
  int k = operation_binary_left[lex.get_value()];
  // cout << "CHECKING " << k << " " << lex.get_value() << " " << searcher.get_current().get_str() << endl;
  return k == priority;
}
