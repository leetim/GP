#pragma once
#include <vector>
#include <sstream>
#include <defs.h>

class Machine;
class Condition;
typedef Machine* PMachine;
typedef Condition* PCondition;

class Lexeme{
public:
  Lexeme(): str(), row(), col(), type(){};
  Lexeme(std::string s, int _row, int _col, int _type): str(s), row(_row), col(_col), type(_type){};
  void print();
private:
  std::string str;
  int row;
  int col;
  int type;
};

class Condition{
public:
  virtual bool check(char c)=0;
  int get_type();
  void set_type(int);
  PCondition get_child(char c);
  PCondition add_child(PCondition);
  void clear();
  virtual ~Condition();
private:
  std::vector<PCondition> childs;
  std::stringstream ss;
  int type;
};

class Condition_scalar: public Condition{
public:
  Condition_scalar(): value(){};
  Condition_scalar(char c): value(c){};
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
  ~Condition_unity();
private:
  std::vector<PCondition> conditions;
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
