#include <machine.h>
#include <stdio.h>

using namespace std;

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

bool Condition_not_scalar::check(char c){
  // printf("'%c' c\n'%c' value\n", c, value);
  return c != value;
}

bool Condition_limits::check(char c){
  // if (greater < littler){
  //   printf("%d %d %d\n", littler, c, greater);
  // }
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

PCondition get_not_scalar(char c){
  return new Condition_not_scalar(c);
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
  return temp;
}

PCondition get_Aa0(){
  vector<PCondition> a = {
    get_A(),
    get_a(),
    get_0()
  };
  PCondition temp = new Condition_unity(a);
  temp->add_child(temp);
  return temp;
}

PCondition get_union(vector<char>& a, int type){
  vector<PCondition> conds;
  for (auto i = a.begin(); i != a.end(); i++){
    conds.push_back(get_scalar(*i));
  }
  PCondition temp = new Condition_unity(conds);
  temp->set_type(type);
  return temp;
}

PCondition get_variable(int type, char c){
  PCondition temp = get_scalar(c);
  temp->add_child(get_a())->add_child(get_Aa0());
  temp->set_type(type);
  return temp;
}

PCondition get_identificate(){
  PCondition temp = get_a();
  temp->add_child(get_Aa0());
  temp->set_type(TYPE_IDENTIFICATE);
  return temp;
}

PCondition get_identificate_type(){
  PCondition temp = get_A();
  temp->add_child(get_Aa0());
  temp->set_type(TYPE_IDENTIFICATE);
  return temp;
}

PCondition get_leteral(){
  PCondition temp1 = get_0();
  PCondition temp2 = get_0();
  temp1->set_type(TYPE_LETERAL);
  temp2->set_type(TYPE_FLOAT_AFTER_POINT);
  temp1->add_child(get_scalar('E'))->add_child(temp2);
  return temp1;
}

PCondition get_spaces(){
  vector<char> a = {
    ' ',
    '\t',
    '\n'
  };
  PCondition temp = get_union(a, TYPE_SPACE);
  temp->add_child(temp);
  return temp;
}

void get_operator(PCondition start){
  vector<string> a = {
    "!",
    "%=",
    "**",
    "--",
    "++",
    ">>",
    "<<",
    "==",
    ">=",
    "<=",
    "/=",
    "...",
    "&&",
    "||",
    "^",
    ";",
    ",",
    ":"
  };
  for (auto i = a.begin(); i != a.end(); i++){
    PCondition cur = start;
    for (int j = 0; j < (*i).size(); j++){
      char c = (*i)[j];
      PCondition temp = cur->get_child(c);
      if (temp == NULL){
        cur = cur->add_child(get_scalar(c));
        cur->set_type(TYPE_OPERATOR);
      }
      else{
        cur = temp;
      }
    }
  }
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
  return get_union(a, TYPE_SEPARATOR);
}

PCondition get_comment(){
  PCondition head = get_scalar('#');
  PCondition tail = get_scalar('\n');
  PCondition body = get_not_scalar('\n');
  body->add_child(body);
  tail->set_type(TYPE_COMMENT);
  // PCondition finish1 = get_scalar('/');
  // finish1->set_type(TYPE_COMMENT);
  head->add_child(body)->add_child(tail);
  // head->add_child(get_scalar('*'))->add_child(get_not_scalar('*'))->add_child(get_scalar('*'))->add_child(finish1);
  return head;
}

PCondition get_eof(){
  PCondition temp = get_scalar((char)34);
  temp->set_type(TYPE_EOF);
  return temp;
}

////////////////////////////////////////////////////////////////////////////////
//Machine

void Machine::learn(){
  start = new Condition_unity;
  start->add_child(get_variable(TYPE_VAR, '$'));
  start->add_child(get_variable(TYPE_ARRAY, '@'));
  start->add_child(get_identificate());
  start->add_child(get_identificate_type());
  start->add_child(get_leteral());
  start->add_child(get_spaces());
  get_operator(start);
  start->add_child(get_separator());
  start->add_child(get_comment());
  start->add_child(get_eof());

  cur = start;
}

void Machine::add_str(string s){
  printf("%s\n", s.c_str());
  current_line = s;
  cur_ind = 0;
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
      // printf("IN NEXT: cur_ind = %d %d %c\n", cur_ind, current_line.size(), current_line[cur_ind]);
      break;
    }
    ss << c;
    cur = temp;
    cur_ind++;
  }
  Lexeme l(ss.str(), line_number, col, cur->get_type());
  cur = start;
  if ((unsigned int)cur_ind == current_line.size() || cur->get_type() == TYPE_EOF){
    // printf("END_LINE: cur_ind = %d %d %c\n//////////////////////\n", cur_ind, current_line.size(), current_line[cur_ind]);
    // l.print();
    throw l;
  }
  return l;
}
