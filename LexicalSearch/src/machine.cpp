#include <machine.h>
#include <stdio.h>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//Lexeme

static string types[8] = {
  "NONE",
  "IDENTIFICATE",
  "LETERAL",
  "SEPARATOR",
  "OPERATOR",
  "SPACE",
  "DIRECT",
  "EOF"
};

void Lexeme::print(){
  printf("LEXEMA FOUNDED: %s, %d, %d, '%s'\n", types[type].c_str(), row, col, str.c_str());
}

////////////////////////////////////////////////////////////////////////////////
//Condition

int Condition::get_type(){
  return type;
}

void Condition::set_type(int t){
  type = t;
  for (auto i = childs.begin(); i != childs.end(); i++){
    if (*i != this){
      (*i)->set_type(t);
    }
  }
}

PCondition Condition::add_child(PCondition cond){
  childs.push_back(cond);
  return cond;
}

PCondition Condition::get_child(char c){
  // printf("Length: %d '%c'\n", childs.size(), c);
  for (auto i = childs.begin(); i != childs.end(); i++){
    if ((*i)->check(c)){
      return *i;
    }
  }
  return NULL;
}


Condition::~Condition(){
  for (auto i = childs.begin(); i != childs.end(); i++){
    if (*i != this){
      delete *i;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
//Condition::check

bool Condition_scalar::check(char c){
  // printf("'%c' c\n'%c' value\n", c, value);
  return c == value;
}

bool Condition_limits::check(char c){
  return (c >= littler) && (c <= greater);
}

bool Condition_unity::check(char c){
  bool result = false;
  for (auto i = conditions.begin(); i != conditions.end(); i++){
    result |= (*i)->check(c);
  }
  return result;
}

////////////////////////////////////////////////////////////////////////////////
//Condition_unity

Condition_unity::Condition_unity(PCondition fst, PCondition scnd){
  conditions.push_back(fst);
  conditions.push_back(scnd);
}

Condition_unity::~Condition_unity(){
  for (auto i = conditions.begin(); i != conditions.end(); i++){
    delete *i;
  }
}

////////////////////////////////////////////////////////////////////////////////
//function for getting conditions
PCondition get_scalar(char c){
  return new Condition_scalar(c);
}

PCondition get_limits(char l, char g){
  return new Condition_limits(l, g);
}

PCondition get_A(){
  return get_limits('A', 'Z');
}

PCondition get_a(){
  return new Condition_unity(get_limits('a', 'z'), get_scalar('_'));
}

PCondition get_0(){
  PCondition temp = get_limits('0', '9');
  temp->add_child(temp);
  temp->set_type(TYPE_LETERAL);
  return temp;
}

PCondition get_Aa0(int type){
  vector<PCondition> a = {
    get_A(),
    get_a(),
    get_0()
  };
  PCondition temp = new Condition_unity(a);
  temp->add_child(temp);
  temp->set_type(type);
  return temp;
}

PCondition get_spaces(){
  vector<PCondition> a = {
    get_scalar(' '),
    get_scalar('\t'),
    get_scalar('\n')
  };
  PCondition temp = new Condition_unity(a);
  temp->add_child(temp);
  temp->set_type(TYPE_SPACE);
  return temp;
}

PCondition get_operator(){
  vector<char> a = {
    '!',
    '%',
    '*',
    '-',
    '+',
    '=',
    '>',
    '<',
    '/',
    '.',
    '&',
    '|',
    '^'
  };
  vector<PCondition> conds;
  for (auto i = a.begin(); i != a.end(); i++){
    conds.push_back(get_scalar(*i));
  }
  PCondition temp = new Condition_unity(conds);
  temp->add_child(temp);
  temp->set_type(TYPE_OPERATOR);
  return temp;
}

PCondition get_separator(){
  vector<char> a = {
    '(',
    ')',
    '[',
    ']',
    '{',
    '}'
  };
  vector<PCondition> conds;
  for (auto i = a.begin(); i != a.end(); i++){
    conds.push_back(get_scalar(*i));
  }
  PCondition temp = new Condition_unity(conds);
  temp->set_type(TYPE_SEPARATOR);
  return temp;
}

////////////////////////////////////////////////////////////////////////////////
//Machine

void Machine::learn(){
  start = new Condition_unity;
  PCondition temp;
  temp = get_scalar('$');
  temp->add_child(get_a())->add_child(get_Aa0(TYPE_IDENTIFICATE));
  temp->set_type(TYPE_IDENTIFICATE);
  start->add_child(temp);

  temp = get_scalar('@');
  temp->add_child(get_a())->add_child(get_Aa0(TYPE_IDENTIFICATE));
  temp->set_type(TYPE_IDENTIFICATE);
  start->add_child(temp);

  temp = get_a();
  temp->add_child(get_Aa0(TYPE_IDENTIFICATE));
  temp->set_type(TYPE_IDENTIFICATE);
  start->add_child(temp);

  temp = get_0();
  // temp->add_child(get_scalar('.'))->add_child(get_0());
  temp->set_type(TYPE_LETERAL);
  start->add_child(temp);

  temp = get_spaces();
  temp->set_type(TYPE_SPACE);
  start->add_child(temp);

  temp = get_operator();
  temp->set_type(TYPE_OPERATOR);
  start->add_child(temp);

  temp = get_separator();
  temp->set_type(TYPE_SEPARATOR);
  start->add_child(temp);


  // start->add_child(get_scalar('$'))->add_child(get_a())->add_child(get_Aa0(TYPE_IDENTIFICATE));
  // start->add_child(get_scalar('@'))->add_child(get_a())->add_child(get_Aa0(TYPE_IDENTIFICATE));
  // start->add_child(get_a())->add_child(get_Aa0(TYPE_IDENTIFICATE));
  // start->add_child(get_0())->add_child(get_scalar('.'))->add_child(get_0());
  // start->add_child(get_spaces());
  // start->add_child(get_operator());
  // start->add_child(get_scalar('#'))->
  // start->add_child(new Condition_scalar('\"'))->
  cur = start;
}

void Machine::add_str(string s){
  current_line = s;
  line_number++;
}

Lexeme Machine::next(){
  stringstream ss;
  col = cur_ind;
  while ((unsigned int)cur_ind < current_line.size()){
    // printf("hi\n");
    char c = current_line[cur_ind];
    PCondition temp = cur->get_child(c);
    if (temp == NULL){
      Lexeme l(ss.str(), line_number, col, cur->get_type());
      cur = start;
      return l;
    }
    ss << c;
    cur = temp;
    cur_ind++;
  }
  throw 2;
}
