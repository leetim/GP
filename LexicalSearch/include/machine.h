#pragma once
#include <vector>
#include <sstream>
#include <defs.h>
#include <memory>

class Machine;
class Condition;
typedef Machine* PMachine;
typedef std::shared_ptr<Condition> PCondition;

class Condition{
public:
  virtual bool check(char c)=0;
  LexemeType get_type();
  PCondition set_type(LexemeType);
  PCondition get_child(char c);
  PCondition add_child(PCondition);
  void clear();
private:
  std::vector<PCondition> childs;
  std::stringstream ss;
  LexemeType type;
};

class Condition_scalar: public Condition{
public:
  Condition_scalar(): value(){};
  Condition_scalar(char c): value(c){};
  bool check(char c);
private:
  char value;
};



class Condition_not_scalar: public Condition{
public:
  Condition_not_scalar(): value(){};
  Condition_not_scalar(char c): value(c){};
  bool check(char c);
private:
  char value;
};

class Condition_limits: public Condition{
public:
  Condition_limits(): littler(), greater(){};
  Condition_limits(char lt, char gt): littler(lt), greater(gt){};
  bool check(char c);
private:
  char littler;
  char greater;
};

class Condition_unity: public Condition{
public:
  Condition_unity(): conditions(){};
  Condition_unity(PCondition fst, PCondition scnd);
  Condition_unity(std::vector<PCondition>& cond): conditions(cond){};
  bool check(char c);
private:
  std::vector<PCondition> conditions;
};

class Condition_intersection: public Condition_unity{
public:
  Condition_intersection(): conditions(){};
  Condition_intersection(PCondition fst, PCondition scnd): Condition_unity(fst, scnd){};
  Condition_intersection(std::vector<PCondition>& cond): conditions(cond){};
  bool check(char c);
private:
  std::vector<PCondition> conditions;
};

class Condition_negative: public Condition{
public:
  Condition_negative(): cond(){};
  Condition_negative(PCondition c): cond(c){};
  bool check(char c);
private:
  PCondition cond;
};

class Machine{
public:
  Machine(): current_line(), cur_ind(), col(), line_number(-1){};
  void learn();
  Lexeme next();
  void add_str(std::string);
private:
  std::string current_line;
  int cur_ind;
  int col;
  int line_number;
  PCondition start;
  PCondition cur;
};
