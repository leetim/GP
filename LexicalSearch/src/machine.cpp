#include <machine.h>

using namespace std;


PCondition Condition::add_child(PCondition cond){
  childs.push_back(cond);
  return cond;
}

PCondition Condition::get_child(char c){
  for (auto i = childs.begin(); i != childs.end(); i++){
    if ((*i)->check(c)){
      return *i;
    }
  }
  return NULL;
}

string Condition::get_str(){
  return ss.str();
}

void Condition::clear(){
  ss.str("");
  for (auto i = childs.begin(); i != childs.end(); i++){
    (*i)->clear();
  }
}

bool Condition_scalar::check(char c){
  return c == value;
}

int Condition_scalar::get_type(){
  return 1;
}

bool Condition_limits::check(char c){
  return (c >= littler) && (c <= greater);
}

int Condition_limits::get_type(){
  return (int)(greater - littler + 1);
}

Condition_unity::Condition_unity(PCondition fst, PCondition scnd){
  conditions.push_back(fst);
  conditions.push_back(scnd);
}


bool Condition_unity::check(char c){
  bool result = true;
  for (auto i = conditions.begin(); i != conditions.end(); i++){
    result &= (*i)->check(c);
  }
  return result;
}

int Condition_unity::get_type(){
  int sum = 0;
  for (auto i = conditions.begin(); i != conditions.end(); i++){
    sum += (*i)->get_type();
  }
  return sum;
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

PCondition get_spaces(){
  PCondition temp = new Condition_unity(get_scalar(' '), get_scalar('\t'));
  temp->add_child(temp);
  return temp;
}

void Machine::learn(){
  start = new Condition_unity;
  start->add_child(get_scalar('$'))->add_child(get_a())->add_child(get_Aa0());
  start->add_child(get_scalar('@'))->add_child(get_a())->add_child(get_Aa0());
  start->add_child(get_a())->add_child(get_Aa0());
  start->add_child(get_0())->add_child(get_scalar('.'))->add_child(get_0());
  start->add_child(get_spaces());
  // start->add_child(new Condition_scalar('\"'))->
  cur = start;
}
