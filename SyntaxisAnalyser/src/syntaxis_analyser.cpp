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
  {"[", 14},
  {".", 13},
  {"*", 12},
  {"/", 11},
  {"%", 11},
  {"+", 10},
  {"-", 10},
  {"<<", 9},
  {">>", 9},
  {"&", 8},
  {"^", 7},
  {"|", 6},
  {"<", 5},
  {"<=", 5},
  {">", 5},
  {">=", 5},
  {"==", 4},
  {"!=", 4},
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
    if (next == LT_OSB){
      PNocle inside = parse_expr(1);
      temp = PNocle(new NocleBinaryLeft(Lexeme("array_element", 0, 0, LT_SEPARATOR), temp, inside));
      require_lexeme(LT_CSB);
      next = searcher.next();
    }
    else{
      PNocle right = parse_expr(priority + 1);
      temp = PNocle(new NocleBinaryLeft(next, temp, right));
      next = searcher.get_current();
    }
  }
  return temp;
}

PNocle SyntaxisAnalyser::parse_factor(){
  Lexeme cur = searcher.get_current();
  // cout << cur.get_str() << " " << cur.is_identificator() << endl;
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
  // cout << next.get_str() << " " << cur.get_str() << endl;
  if (cur == LT_OP){
    PNocle temp = parse_expr(1);
    require_lexeme(LT_CP);
    searcher.next();
    return temp;
  }
  if (cur == LT_EOF){
    return PNocle(new Nocle(cur));
  }
  throw Errors::Illegal_expression(cur); //Нужна адекватная ошибка
  // return NULL;
}

PNocle SyntaxisAnalyser::parse_function(PNocle ident, LexemeType finish){
  NocleMulty* res = new NocleMulty(Lexeme("function", 0, 0, LT_SEPARATOR));
  res->add_child(ident);
  while (true){
    // cout << "REQ" << endl;
    PNocle temp = parse_expr();
    res->add_child(temp);
    // cout << searcher.get_current().get_str() << endl;
    if (searcher.get_current().get_str() != ","){
      break;
    }
    searcher.next();
  }
  // cout << "REQ " << next.get_str() << endl;
  if (finish != LT_NONE){
    require_lexeme(finish);
    searcher.next();
  }
  return PNocle(res);
}

PNocle SyntaxisAnalyser::parse_identificator(){
  Lexeme cur = searcher.get_current();
  Lexeme next = searcher.next_with_spaces();
  PNocle ident(new NocleTerminate(cur));
  if (next.get_type() == LT_SPACE){
    next = searcher.next();
    if (!(next == LT_OPERATOR)){
      return parse_function(ident, LT_NONE);
    }
  }
  if (next == LT_OP){
    searcher.next();
    return parse_function(ident, LT_CP);
  }
  return ident;
}

bool SyntaxisAnalyser::require_lexeme(LexemeType lt){
  if (searcher.get_current() == lt){
    return true;
  }
  throw Errors::ClosingParenthesisNotFound(searcher.get_current());
}


bool SyntaxisAnalyser::check_priority(Lexeme lex, int priority){
  int k = operation_binary_left[lex.get_value()];
  // cout << "CHECKING " << k << " " << lex.get_value() << " " << searcher.get_current().get_str() << endl;
  return k == priority;
}
