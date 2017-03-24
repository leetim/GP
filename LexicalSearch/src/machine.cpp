#include <machine.h>
#include <stdio.h>
#include <iostream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
//Condition

LexemeType Condition::get_type(){
  return type;
}

PCondition Condition::set_type(LexemeType t){
  type = t;
  for (auto i = childs.begin(); i != childs.end(); i++){
    if ((*i)->type != this->type){
      (*i)->set_type(t);
    }
  }
  PCondition temp;
  return temp;
}

PCondition Condition::add_child(PCondition cond){
  childs.push_back(cond);
  return cond;
}

PCondition Condition::get_child(char c){
  for (auto i = childs.begin(); i != childs.end(); i++){
    if ((*i)->check(c)){
      // printf("Length: %d '%c'\n", childs.size(), c);
      return *i;
    }
  }
  return PCondition();
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

////////////////////////////////////////////////////////////////////////////////
//Condition_intersection

bool Condition_intersection::check(char c){
  bool result = true;
  for (auto i = conditions.begin(); i != conditions.end(); i++){
    result &= (*i)->check(c);
  }
  return result;
}

////////////////////////////////////////////////////////////////////////////////
//Condition_negative

bool Condition_negative::check(char c){
  return !cond->check(c);
}

////////////////////////////////////////////////////////////////////////////////
//function for getting conditions

PCondition get_scalar(char c){
  return PCondition(new Condition_scalar(c));
}

PCondition get_limits(char l, char g){
  return PCondition(new Condition_limits(l, g));
}

PCondition get_not_scalar(char c){
  return PCondition(new Condition_not_scalar(c));
}

PCondition get_A(){
  return get_limits('A', 'Z');
}

PCondition get_a(){
  return PCondition(new Condition_unity(get_limits('a', 'z'), get_scalar('_')));
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
  PCondition temp(new Condition_unity(a));
  temp->add_child(temp);
  return temp;
}

PCondition get_negative(PCondition c){
  return PCondition(new Condition_negative(c));
}

PCondition get_union(vector<char>& a, LexemeType type){
  vector<PCondition> conds;
  for (auto i = a.begin(); i != a.end(); i++){
    conds.push_back(get_scalar(*i));
  }
  PCondition temp(new Condition_unity(conds));
  temp->set_type(type);
  return temp;
}

PCondition get_union(char frst, char sec){
  vector<char> v;
  v.push_back(frst);
  v.push_back(sec);
  return get_union(v, LT_NONE);
}

PCondition get_variable(LexemeType type, char c){
  PCondition temp = get_scalar(c);
  temp->add_child(get_Aa0());
  temp->set_type(type);
  return temp;
}

PCondition get_identificate(){
  PCondition temp = get_a();
  temp->add_child(get_Aa0());
  temp->set_type(LT_IDENTIFICATE);
  return temp;
}

PCondition get_identificate_type(){
  PCondition temp = get_A();
  temp->add_child(get_Aa0());
  temp->set_type(LT_TYPE_NAME);
  return temp;
}

PCondition get_leteral(){
  // PCondition none_other(new Condition_unity(get_A(), get_a()));
  //   none_other->set_type(LT_NONE);
  PCondition temp = get_0();
    temp->set_type(LT_INT);
  PCondition point = get_scalar('.');
    point->set_type(LT_FLOAT_POINT);
  PCondition point1 = get_scalar('.');
    point1->set_type(LT_FLOAT_POINT);
  PCondition temp1 = get_0();
    temp1->set_type(LT_FLOAT);
  PCondition none_e(new Condition_unity(get_scalar('E'), get_scalar('e')));
    none_e->set_type(LT_NONE);
  PCondition temp2 = get_0();
    temp2->set_type(LT_FLOAT);
  temp->add_child(point)->add_child(temp1)->add_child(none_e)->add_child(temp2);
  point->add_child(point1);
  point1->add_child(point1);

  // temp->add_child(none_other);
  // temp1->add_child(none_other);
  // none_e->add_child(none_other);
  // temp2->add_child(none_other);
  return temp;
}

PCondition get_leteral_str(){
  PCondition temp1 = get_scalar('\'');
  PCondition temp2 = get_not_scalar('\'');
  temp2->add_child(temp2);
  temp1->add_child(temp2)->add_child(get_scalar('\''));
  temp1->add_child(get_scalar('\''));
  temp1->set_type(LT_STRING);
  return temp1;
}

PCondition get_active_str(){
  PCondition temp = get_scalar('\"');
  PCondition finish = get_scalar('\"');
  finish->set_type(LT_ACTIVE_STRING);
  PCondition body = get_negative(get_union('\"', '\\'));
  body->add_child(body);
  PCondition escape = get_scalar('\\');
  PCondition escape_body = escape->add_child(get_negative(get_scalar('\0')));
  escape_body->add_child(body);
  escape_body->add_child(finish);
  escape_body->add_child(escape);
  temp->add_child(body);
  temp->add_child(finish);
  temp->add_child(escape);
  return temp;
}

PCondition get_spaces(){
  vector<char> a = {
    ' ',
    '\t',
    '\n'
  };
  PCondition temp = get_union(a, LT_SPACE);
  temp->add_child(temp);
  return temp;
}

void get_operator(PCondition start){
  vector<string> a = {
    "!=",
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
    // ";",
    ",",
    ":",
    "+=",
    "-=",
    "*=",
    "|=",
    "&="
  };
  for (auto i = a.begin(); i != a.end(); i++){
    PCondition cur = start;
    for (unsigned int j = 0; j < (*i).size(); j++){
      char c = (*i)[j];
      PCondition temp = cur->get_child(c);
      if (temp == NULL){
        cur = cur->add_child(get_scalar(c));
        cur->set_type(LT_OPERATOR);
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
  return get_union(a, LT_SEPARATOR);
}

PCondition get_comment(){
  PCondition head = get_scalar('#');
  PCondition tail = get_scalar('\n');
  PCondition body = get_not_scalar('\n');
  body->add_child(body);
  tail->set_type(LT_COMMENT);
  head->add_child(body)->add_child(tail);
  head->add_child(tail);
  return head;
}

PCondition get_eof(){
  PCondition temp = get_scalar((char)3);
  temp->set_type(LT_EOF);
  return temp;
}

////////////////////////////////////////////////////////////////////////////////
//Machine

void Machine::learn(){
  start = PCondition(new Condition_unity);
  cur = start;
  start->add_child(get_variable(LT_VAR, '$'));
  start->add_child(get_variable(LT_ARRAY, '@'));
  start->add_child(get_identificate());
  start->add_child(get_identificate_type());
  start->add_child(get_leteral());
  start->add_child(get_leteral_str());
  start->add_child(get_spaces());
  get_operator(start);
  start->add_child(get_separator());
  start->add_child(get_comment());
  start->add_child(get_eof());
}

void Machine::add_str(string s){
  // printf("%s\n", s.c_str());
  current_line = s;
  cur_ind = 0;
  line_number++;
}

Lexeme Machine::next(){
  stringstream ss;
  col = cur_ind;
  while ((unsigned int)cur_ind < current_line.size()){
    char c = current_line[cur_ind];
    PCondition temp = cur->get_child(c);
    if (temp == NULL){
      break;
    }
    ss << c;
    cur = temp;
    cur_ind++;
  }
  // cout << cur->get_type() << endl;
  Lexeme l(ss.str(), line_number, col, cur->get_type());
  cur = start;
  if ((unsigned int)cur_ind == current_line.size()){
    throw Errors::End_of_line(l);
  }
  // cout << cur->get_type() << " " << LT_EOF << endl;
  switch(l.get_type()){
    // case LT_EOF:
    //   throw Errors::End_of_file(l);
    case LT_NONE:
      throw Errors::Unknown_lexeme(l);
    default:
      return l;
  }
}
