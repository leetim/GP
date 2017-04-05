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
  {".", 15},
  {"[", 14},
  {"..", 13},
  {"...", 13},
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

PSymbolTable get_global_table(){
  PSymbolTable global = PSymbolTable(new SymbolTable());
  Symbol::PType str(new Symbol::String());
  Symbol::PType arr(new Symbol::Array(str, -1));
  global->add_symbol(Symbol::PBase(new Symbol::Variable("$", arr)));
  return global;
}

PNocle SyntaxisAnalyser::get_tree(){
  searcher.next();
  PSymbolTable st = get_global_table();
  PNocle temp = parse_statment(st);
  // st->print();
  return temp;
}

PNocle SyntaxisAnalyser::parse_statment(PSymbolTable t){
  PNocle temp;
  Lexeme cur = searcher.get_current();
  // cur.print();
  switch (cur.get_type()){
    case LT_DO:
      searcher.next();
      temp = parse_block(t);
      break;
    case LT_FOR:
      temp = parse_for_cycle(t);
      break;
    case LT_WHILE:
      temp = parse_while_cycle(t);
      break;
    case LT_IF:
      temp = parse_if(t);
      break;
    case LT_VAR:
      temp = parse_def_variable(t);
      break;
    default:
      temp = parse_simple_statment(t);
  }
  return temp;
}

PNocle SyntaxisAnalyser::parse_block(PSymbolTable t, LexemeType end){
  Lexeme cur = searcher.get_current();
  PSymbolTable table(new SymbolTable(t));
  NocleMulty* res = new NocleBlock(table);
  // cur.print();
  while (cur != end && cur != LT_END){
    res->add_child(parse_statment(table));
    cur = searcher.get_current();
  }
  searcher.next();
  cout << "Table:" << endl;
  table->print();
  return PNocle(res);
}

PNocle SyntaxisAnalyser::parse_while_cycle(PSymbolTable t){
  searcher.next();
  PNocle expr = parse_expr();
  // cout << "123" << endl;
  searcher.get_current().print();
  PNocle block = parse_block(t);
  NocleWhile* nif = new NocleWhile(expr, block);
  return PNocle(nif);
}

PNocle SyntaxisAnalyser::parse_for_cycle(PSymbolTable t){
  PSymbolTable st(t);
  Lexeme var = searcher.next();
  searcher.next();
  require_lexeme(LT_IN);
  searcher.next();
  PNocle range = parse_expr();
  // Добавить типы в таблицу символов
  PNocle block = parse_block(st);
  NocleFor* nf = new NocleFor(st, range, block);
  return PNocle(nf);
}

PNocle SyntaxisAnalyser::parse_if(PSymbolTable t){
    searcher.next();
    PNocle expr = parse_expr();
    searcher.get_current().print();
    PNocle block_if = parse_block(t, LT_ELSE);
    PNocle block_else;
    if (searcher.get_current() == LT_ELSE){
      // searcher.next();
      block_else = parse_block(t);
    }
    else{
      block_else = PNocle(new Nocle());
    }
    NocleIf* nif = new NocleIf(expr, block_if, block_else);
    return PNocle(nif);
}

PNocle SyntaxisAnalyser::parse_def_variable(PSymbolTable t){
  Lexeme name = searcher.next();
  searcher.next();
  require_lexeme(LT_COLON);
  Lexeme type_name = searcher.next();
  Lexeme assigment = searcher.next();
  Symbol::Variable* var = new Symbol::Variable(name.get_str(), t->get_type_from_str(type_name.get_str()));
  t->add_symbol(Symbol::PBase(var));
  // cout << t->get_count() << endl;
  if (assigment.get_str() == "="){
    searcher.next();
    PNocle expr = parse_expr();
    // cout << expr->get_str() << endl;
    PNocle term(new NocleTerminate(name));
    return PNocle(new NocleBinary(assigment, term, expr));
  }
  return PNocle(new Nocle());
}

PNocle SyntaxisAnalyser::parse_expr(int priority){
  if (priority > 16){
    // cout << priority << " ";
    // searcher.get_current().print();
    return parse_factor();
  }
  PNocle temp = parse_expr(priority + 1);
  Lexeme next = searcher.get_current();
  while(check_priority(next, priority)){
    searcher.next();
    if (next == LT_OSB){
      PNocle inside = parse_expr(1);
      temp = PNocle(new NocleArrayElement(temp, inside));
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
  if (cur.is_identificator()){
    return parse_identificator();
  }
  Lexeme next = searcher.next();
  if (cur.is_leteral()){
    return PNocle(new NocleTerminate(cur));
  }
  // cout << cur.get_type() << " " << cur.is_identificator() << endl;
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
  // if (cur == LT_EOF){
  //   return PNocle(new Nocle(cur));
  // }
  throw Errors::Illegal_expression(cur); //Нужна адекватная ошибка
  // return NULL;
}

PNocle SyntaxisAnalyser::parse_function(PNocle ident, LexemeType finish){
  NocleMulty* res = new NocleFunction();
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
  if (next == LT_SPACE){
    next = searcher.next();
    if (!(next == LT_OPERATOR || next == LT_ASSIGMENT || next.is_close_separator())){
      return parse_function(ident, LT_NONE);
    }
  }
  if (next == LT_NEW_LINE){
    next = searcher.next();
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

PNocle SyntaxisAnalyser::parse_simple_statment(PSymbolTable t){
  PNocle temp = parse_expr();
  Lexeme cur = searcher.get_current();
  // cur.print();
  while (operation_binary_right[cur.get_str()]){
    searcher.next();
    temp = PNocle(new NocleBinaryRight(cur, temp, parse_simple_statment()));
    cur = searcher.get_current();
    // cur.print();
  }
  return temp;
}

bool SyntaxisAnalyser::check_priority(Lexeme lex, int priority){
  int k = operation_binary_left[lex.get_value()];
  // cout << "CHECKING " << k << " " << lex.get_value() << " " << searcher.get_current().get_str() << endl;
  return k == priority;
}
