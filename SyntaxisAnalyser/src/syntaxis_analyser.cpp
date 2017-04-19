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
  {"[", 15},
  {".", 14},
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
  // {"+=", 15},
  // {"-=", 15},
  // {"*=", 15},
  // {"/=", 15},
  // {"%=", 15},
  // {"&=", 15},
  // {"|=", 15},
  // {"^=", 15}
};

////////////////////////////////////////////////////////////////////////////////
//functions

PNocle get_binary_Nocle(Lexeme lex, PNocle left, PNocle right){
  Nocle* noc;
  switch (lex.get_type()){
    case LT_RANGE:
      noc = new NocleBinaryRange(lex, left, right);
      break;
    default:
      noc = new NocleBinary(lex, left, right);
  }
  return PNocle(noc);
}

PSymbolTable get_global_table(){
  PSymbolTable global = PSymbolTable(new SymbolTable());
  Symbol::PType str(new Symbol::String());
  Symbol::PType arr(new Symbol::Array(str, -1));
  global->add_symbol(Symbol::PBase(new Symbol::Variable("$", arr)));
  return global;
}

int get_int_from_lexeme(Lexeme lex){
  if (lex.get_type() == LT_INT){
    stringstream ss;
    ss << lex.get_str();
    int res;
    ss >> res;
    return res;
  }
  throw Errors::WrongType(lex);
}

Symbol::PType get_type_from_nocle(PNocle node, PSymbolTable t){
  NocleBinary* nb = (NocleBinary*)node.get();
  if (node->get_lexeme().get_type() == LT_ARRAY_ELEMENT){
    return Symbol::PType(new Symbol::Array(
      get_type_from_nocle(nb->get_child1(), t),
      get_int_from_lexeme(nb->get_child2()->get_lexeme())
    ));
  }
  else{
    if (node->get_lexeme().get_type() == LT_TYPE_NAME){
      return t->get_type_from_str(node->get_lexeme().get_str());
    }
    else{
      throw Errors::WrongType(node->get_lexeme());
    }
  }
  return NULL;
}

PNocle get_nocle_leteral(Lexeme cur){
  Nocle* temp;
  switch(cur.get_type()){
    case LT_INT:
      temp = new NocleInteger(cur);
      break;
    case LT_FLOAT:
      temp = new NocleFloat(cur);
      break;
    case LT_STRING:
      temp = new NocleString(cur);
      break;
    case LT_TRUE:
    case LT_FALSE:
      temp = new NocleBool(cur);
      break;
    default:
      cur.print();
      throw 2.0;
  }
  return PNocle(temp);
}

PNocle get_nocle_identificator(Lexeme cur){
  Nocle* temp;
  switch(cur.get_type()){
    case LT_FUNCTION_NAME:
    case LT_VARIABLE:
      // cur.print();
      temp = new NocleVariable(cur);
      // cout << temp->check_lvalue(NULL) << endl;
      break;
      // temp = new NocleVariable(cur);
    case LT_TYPE_NAME:
      temp = new NocleTypeName(cur);
      break;
  }
  return PNocle(temp);
}

PNocle get_nocle_bin_operator(Lexeme cur, PNocle left, PNocle right){
  Nocle* temp;
  switch (cur.get_type()){
    case LT_ADD:
    case LT_SUB:
    case LT_MULT:
    case LT_DIV:
      temp = new NocleBinaryArithmetic(cur, left, right);
      break;
    case LT_LOG_AND:
    case LT_LOG_OR:
      temp = new NocleBinaryLogic(cur, left, right);
      break;
    case LT_EQ:
    case LT_NEQ:
    case LT_LT:
    case LT_LTEQ:
    case LT_GT:
    case LT_GTEQ:
      temp = new NocleBinaryCompare(cur, left, right);
      break;
    case LT_BIN_AND:
    case LT_BIN_OR:
    case LT_BIN_XOR:
    case LT_SHL:
    case LT_SHR:
      temp = new NocleBinaryBitwice(cur, left, right);
      break;
    case LT_POINT:
      temp = new NocleBinaryPoint(cur, left, right);
      break;
    case LT_RANGE:
    case LT_OPEN_RANGE:
      temp = new NocleBinaryRange(cur, left, right);
      break;
    default:
      cur.print();
      throw Errors::WrongBinaryOperator(cur);
  }
  return PNocle(temp);
}

////////////////////////////////////////////////////////////////////////////////
//SyntaxisAnalyser

PNocle SyntaxisAnalyser::get_tree(){
  searcher.next();
  PSymbolTable st = get_global_table();
  // searcher.next();
  // parse_def_function(st);
  PNocle temp = parse_statment(st);
  cout << "start checking" << endl;
  temp->check_types(st);
  // st->print();
  return temp;
}

PNocle SyntaxisAnalyser::parse_program(){
  searcher.next();
  PSymbolTable st = get_global_table();
  NocleBlock* block = new NocleBlock(st);
  // searcher.get_current().print();
  while (true){
    PNocle temp;
    switch (searcher.get_current().get_type()){
      case LT_TYPE:
        searcher.next();
        temp = parse_type(st);
        break;
      case LT_CLASS:
        searcher.next();
        temp = parse_class(st);
        break;
      case LT_DEF:
        searcher.next();
        temp = parse_def_function(st);
        break;
      case LT_VAR:
      case LT_LET:
      case LT_REF:
        searcher.next();
        temp = parse_def_variable(st);
        // cout << "tut2" << endl;
        // searcher.get_current().print();
        break;
      case LT_DO:
        searcher.next();
        temp = parse_block(st);
        break;
      case LT_EOF:
        block->print_table();
        cout << "start checking" << endl;
        block->check_types(st);
        return PNocle(block);
    }
    block->add_child(temp);
  }
  throw PNocle();
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
    case LT_LET:
    case LT_REF:
    case LT_VAR:
      searcher.next();
      temp = parse_def_variable(t);
      break;
    case LT_RETURN:
      searcher.next();
      return PNocle(new NocleUnaryReturn(cur, parse_simple_statment()));
    case LT_PUTS:
      searcher.next();
      return PNocle(new NocleUnaryPuts(cur, parse_simple_statment()));
    case LT_PRINT:
      searcher.next();
      return PNocle(new NocleUnaryPrint(cur, parse_simple_statment()));
    default:
      temp = parse_simple_statment(t);
  }
  // searcher.get_current().print();
  return temp;
}

PNocle SyntaxisAnalyser::parse_class(PSymbolTable t){
  Lexeme name = searcher.get_current();
  searcher.next();
  PSymbolTable inside(new SymbolTable());
  while (searcher.get_current() != LT_END){
    switch(searcher.get_current().get_type()){
      case LT_LET:
      case LT_VAR:
        searcher.next();
        parse_def_variable(inside);
        break;
      default:
        throw 1; // Need cool error
    }
  }
  // searcher.get_current().print();
  searcher.next();
  t->add_symbol(Symbol::PBase(new Symbol::Record(
    name.get_str(),
    inside
  )));
  // cout << "tut" << endl;
  return PNocle(new Nocle());
}

PNocle SyntaxisAnalyser::parse_type(PSymbolTable t){
  Lexeme name = searcher.get_current();
  Lexeme assigment = searcher.next();
  require_lexeme(LT_ASSIGMENT);
  searcher.next();
  PNocle type = parse_expr();
  // searcher.get_current().print();
  Symbol::Alias* alias = new Symbol::Alias(name.get_str(), get_type_from_nocle(type, t));
  t->add_symbol(Symbol::PBase(alias));
  return PNocle(new Nocle());
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
  PSymbolTable st( new SymbolTable(t));
  Lexeme var = searcher.next();
  searcher.next();
  require_lexeme(LT_IN);
  searcher.next();
  PNocle range = parse_expr();
  st->add_symbol(Symbol::PBase(
    new Symbol::Variable(var.get_str(), range->get_result_type(t))));
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
  Lexeme name = searcher.get_current();
  require_lexeme(LT_VARIABLE);
  searcher.next();
  require_lexeme(LT_COLON);
  searcher.next();
  PNocle type = parse_expr();
  Lexeme assigment = searcher.get_current();
  Symbol::Variable* var;
  // searcher.get_current().print();
  if (assigment.get_str() == "="){
    searcher.next();
    PNocle expr = parse_simple_statment();
    var = new Symbol::Variable(name.get_str(), get_type_from_nocle(type, t), expr);
  }
  else{
    var = new Symbol::Variable(name.get_str(), get_type_from_nocle(type, t));
  }
  t->add_symbol(Symbol::PBase(var));
  // cout << t->get_count() << endl;
  return PNocle(new Nocle());
}

PNocle SyntaxisAnalyser::parse_def_function(PSymbolTable t){
  Lexeme name = searcher.get_current();
  PSymbolTable func_table(new SymbolTable(t));
  bool parenthesis_flag = false;
  bool func_flag = false;
  if (searcher.next_with_spaces() != LT_NEW_LINE){
    if (searcher.get_current() == LT_SPACE){
      // searcher.next();
    }
    if (searcher.get_current() == LT_OP){
      searcher.next();
      parenthesis_flag = true;
    }
    do{
      if (searcher.get_current() == LT_ARROW){
        // searcher.get_current().print();
        Lexeme type_name = searcher.next();
        require_lexeme(LT_TYPE_NAME);
        Symbol::PType type = t->get_type_from_str(type_name.get_str());
        func_table->add_symbol(Symbol::PBase(new Symbol::Variable("$", type)));
        func_flag = true;
        break;
      }
      searcher.next();
      if (searcher.get_current() == LT_LET || searcher.get_current() == LT_REF){
        searcher.next();
      }
      parse_def_variable(func_table);
    } while (searcher.get_current() == LT_COMMA || searcher.get_current() == LT_ARROW);
    if (parenthesis_flag){
      require_lexeme(LT_CP);
      searcher.next();
    }
  }
  Symbol::Procedure* func;
  if (func_flag){
    func = new Symbol::Function(name.get_str(), func_table);
  }
  else{
    func = new Symbol::Procedure(name.get_str(), func_table);
  }
  t->add_symbol(Symbol::PBase(func));
  PNocle block = parse_block(func_table);
  func->set_block(block);
  func_table->print();
  cout << block->get_str() << endl;
  return PNocle(new Nocle());
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
      temp = PNocle(new NocleArrayElement(temp, inside));
      require_lexeme(LT_CSB);
      next = searcher.next();
    }
    else{
      PNocle right = parse_expr(priority + 1);
      temp = get_nocle_bin_operator(next, temp, right);
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
    return get_nocle_leteral(cur);
  }
  // cout << cur.get_type() << " " << cur.is_identificator() << endl;
  if (operation_unary_prefix[cur.get_str()]){
    return PNocle(new NocleUnary(cur, parse_factor()));
  }
  // cout << next.get_str() << " " << cur.get_str() << endl;
  if (cur == LT_OP){
    PNocle temp = parse_expr(1);
    require_lexeme(LT_CP);
    searcher.next();
    return temp;
  }
  if (cur == LT_OB){
    NocleArrayConst* nae = new NocleArrayConst();
    cur = searcher.get_current();
    while (true){
      nae->add_child(parse_expr());
      if (searcher.get_current() == LT_CB){
        break;
      }
      cur = searcher.next();
    }
    require_lexeme(LT_CB);
    searcher.next();
    return PNocle(nae);
  }
  // if (cur == LT_EOF){
  //   return PNocle(new Nocle(cur));
  // }
  throw Errors::Illegal_expression(cur); //Нужна адекватная ошибка
  // return NULL;
}

PNocle SyntaxisAnalyser::parse_function_call(){
  // searcher.get_current().print();
  Lexeme cur = searcher.get_current();
  Lexeme next = searcher.next_with_spaces();
  NocleMulty* res = new NocleFunction();
  res->add_child(get_nocle_identificator(cur));
  if (next == LT_SPACE || next == LT_NEW_LINE || next == LT_OP){
    searcher.next();
    if (next == LT_OPERATOR || next == LT_ASSIGMENT
      || next == LT_NEW_LINE || next.is_close_separator()){
      return PNocle(res);
    }
  }
  while (true){
    PNocle temp = parse_expr();
    res->add_child(temp);
    if (searcher.get_current() != LT_COMMA){
      break;
    }
    searcher.next();
  }
  if (next == LT_OP){
    require_lexeme(LT_CP);
    searcher.next();
  }
  return PNocle(res);
}

PNocle SyntaxisAnalyser::parse_cast(){
  searcher.get_current().print();
  Lexeme cur = searcher.get_current();
  Lexeme next = searcher.next_with_spaces();
  switch (next.get_type()){
    case LT_SPACE:
    case LT_OP:
      searcher.next();
      break;
    case LT_NEW_LINE:
      searcher.next();
    default:
      return get_nocle_identificator(cur);
  }
  PNocle casted = parse_expr();
  if (next == LT_OP){
    require_lexeme(LT_CP);
  }
  NocleUnaryCast* res = new NocleUnaryCast(cur, casted);
  return PNocle(res);
}

PNocle SyntaxisAnalyser::parse_identificator(){
  Lexeme cur = searcher.get_current();
  if (cur == LT_TYPE_NAME){
    searcher.get_current().print();
    return parse_cast();
  }
  if (cur == LT_FUNCTION_NAME){
    return parse_function_call();
  }
  else{
    searcher.next();
    return get_nocle_identificator(cur);
  }
}

bool SyntaxisAnalyser::require_lexeme(LexemeType lt){
  if (searcher.get_current() == lt){
    return true;
  }
  switch (lt){
    case LT_CP:
      throw Errors::ClosingParenthesisNotFound(searcher.get_current());
    default:
      cout << lt << endl;
      searcher.get_current().print();
      throw lt;
  }
}

PNocle SyntaxisAnalyser::parse_simple_statment(PSymbolTable t){
  // Lexeme cur = searcher.get_current();
  // searcher.next();
  PNocle temp = parse_expr();
  Lexeme cur = searcher.get_current();

  while (operation_binary_right[cur.get_str()]){
    searcher.next();
    temp = PNocle(new NocleBinaryAssigment(cur, temp, parse_expr()));
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
