#pragma once

#include <defs.h>
#include <string>
#include <sstream>
#include <memory>
#include <symbol.h>
#include <symbol_table.h>

class Nocle;
enum NocleType{
  NT_BASE,
  NT_TERMINATE,
  NT_NOT_TERMINATE,
  NT_OPERATORS,
  NT_EXPR,
  NT_UNARY_PREFIX,
  NT_UNARY_POSTFIX,
  NT_BINARY_LEFT,
  NT_BINARY_RIGHT
};

class Nocle{
public:
  Nocle(): lexeme(){};
  Nocle(Lexeme l): lexeme(l){};
  void set_lexeme(Lexeme lex);
  Lexeme get_lexeme();
  std::string get_str();
  virtual Symbol::PType get_result_type(PSymbolTable t);
  virtual void get_str(std::stringstream& ss, int depth = 0);
  virtual bool check_lvalue(PSymbolTable);
  virtual bool check_types(PSymbolTable);
protected:
  bool lvalue_flag = false;
  bool check_flag = false;
private:
  Lexeme lexeme;
};
// typedef std::shared_ptr<Nocle> PNocle;
typedef std::shared_ptr<Nocle> PNocle;

////////////////////////////////////////////////////////////////////////////////
//NocleTerminate

class NocleTerminate: public Nocle{
public:
  NocleTerminate(): Nocle(){};
  NocleTerminate(Lexeme lex): Nocle(lex){};
  virtual Symbol::PType get_result_type(PSymbolTable t);
  virtual void get_str(std::stringstream& ss, int depth = 0);
};

class NocleInteger: public NocleTerminate{
public:
  NocleInteger(): NocleTerminate(){};
  NocleInteger(Lexeme lex): NocleTerminate(lex){};
  Symbol::PType get_result_type(PSymbolTable t);
  // virtual void get_str(std::stringstream& ss, int depth = 0);
};

class NocleBool: public NocleTerminate{
public:
  NocleBool(): NocleTerminate(){};
  NocleBool(Lexeme lex): NocleTerminate(lex){};
  Symbol::PType get_result_type(PSymbolTable t);
  // virtual void get_str(std::stringstream& ss, int depth = 0);
};

class NocleFloat: public NocleTerminate{
public:
  NocleFloat(): NocleTerminate(){};
  NocleFloat(Lexeme lex): NocleTerminate(lex){};
  Symbol::PType get_result_type(PSymbolTable t);
  // virtual void get_str(std::stringstream& ss, int depth = 0);
};

class NocleString: public NocleTerminate{
public:
  NocleString(): NocleTerminate(){};
  NocleString(Lexeme lex): NocleTerminate(lex){};
  Symbol::PType get_result_type(PSymbolTable t);
  // virtual void get_str(std::stringstream& ss, int depth = 0);
};

class NocleVariable: public NocleTerminate{
public:
  NocleVariable(): NocleTerminate(){};
  NocleVariable(Lexeme lex): NocleTerminate(lex){};
  Symbol::PType get_result_type(PSymbolTable t);
  bool check_lvalue(PSymbolTable);
  // virtual void get_str(std::stringstream& ss, int depth = 0);
};


class NocleTypeName: public NocleTerminate{
public:
  NocleTypeName(): NocleTerminate(){};
  NocleTypeName(Lexeme lex): NocleTerminate(lex){};
  Symbol::PType get_result_type(PSymbolTable t);
  // virtual void get_str(std::stringstream& ss, int depth = 0);
};

////////////////////////////////////////////////////////////////////////////////
//NocleUnary

class NocleUnary: public Nocle{
public:
  NocleUnary(): Nocle(){};
  NocleUnary(Lexeme lex, PNocle ch = NULL): Nocle(lex), child(ch){};
  void set_child(PNocle ch);
  PNocle get_child();
  void get_str(std::stringstream& ss, int depth = 0);
  virtual Symbol::PType get_result_type(PSymbolTable t);
private:
  PNocle child;
};

class NocleUnarySub: public NocleUnary{
public:
  NocleUnarySub(): NocleUnary(){};
  NocleUnarySub(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
};

class NocleUnaryAdd: public NocleUnary{
public:
  NocleUnaryAdd(): NocleUnary(){};
  NocleUnaryAdd(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
};

// class NocleUnaryAddAdd: public NocleUnary{
// public:
//   NocleUnaryAddAdd(): NocleUnary(){};
//   NocleUnaryAddAdd(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
// };

// class NocleUnarySubSub: public NocleUnary{
// public:
//   NocleUnarySubSub(): NocleUnary(){};
//   NocleUnarySubSub(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
// };

class NocleUnaryNot: public NocleUnary{
public:
  NocleUnaryNot(): NocleUnary(){};
  NocleUnaryNot(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
};

class NocleUnaryCast: public NocleUnary{
public:
  NocleUnaryCast(): NocleUnary(){};
  NocleUnaryCast(Lexeme lex, PNocle ch = NULL);
  NocleUnaryCast(Symbol::PType, PNocle ch = NULL);
  Symbol::PType get_result_type(PSymbolTable t);
private:
  Symbol::PType cast_to;
};

class NocleUnaryReturn: public NocleUnary{
public:
  NocleUnaryReturn(): NocleUnary(){};
  NocleUnaryReturn(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
};

class NocleUnaryPuts: public NocleUnary{
public:
  NocleUnaryPuts(): NocleUnary(){};
  NocleUnaryPuts(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
};

class NocleUnaryPrint: public NocleUnary{
public:
  NocleUnaryPrint(): NocleUnary(){};
  NocleUnaryPrint(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
};

////////////////////////////////////////////////////////////////////////////////
//NocleBinary

class NocleBinary: public Nocle{
public:
  NocleBinary(): Nocle(){};
  NocleBinary(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): Nocle(lex), child1(ch1), child2(ch2){};
  void set_child1(PNocle child);
  PNocle get_child1();
  void set_child2(PNocle child);
  PNocle get_child2();
  void get_str(std::stringstream& ss, int depth = 0);
  virtual bool check_types(PSymbolTable t);
  virtual Symbol::PType get_result_type(PSymbolTable t);
protected:
  virtual bool check_child_type(BaseSymbolType bst1, BaseSymbolType bst2);
  PNocle child1;
  PNocle child2;
};

class NocleBinaryLogic: public NocleBinary{
public:
  NocleBinaryLogic(): NocleBinary(){};
  NocleBinaryLogic(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL):
    NocleBinary(lex, ch1, ch2){};
protected:
  virtual bool check_child_type(BaseSymbolType bst1, BaseSymbolType bst2);
};

class NocleBinaryArithmetic: public NocleBinary{
public:
  NocleBinaryArithmetic(): NocleBinary(){};
  NocleBinaryArithmetic(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL):
    NocleBinary(lex, ch1, ch2){};
protected:
  virtual bool check_child_type(BaseSymbolType bst1, BaseSymbolType bst2);
};

class NocleBinaryBitwice: public NocleBinary{
public:
  NocleBinaryBitwice(): NocleBinary(){};
  NocleBinaryBitwice(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL):
    NocleBinary(lex, ch1, ch2){};
protected:
  virtual bool check_child_type(BaseSymbolType bst1, BaseSymbolType bst2);
};

class NocleBinaryOther: public NocleBinary{
public:
  NocleBinaryOther(): NocleBinary(){};
  NocleBinaryOther(Lexeme lex, PNocle ch1, PNocle ch2, Symbol::PType tp = NULL):
    NocleBinary(lex, ch1, ch2), result_type(tp){};
    virtual Symbol::PType get_result_type(PSymbolTable t);
protected:
  virtual Symbol::PType get_wanted_result_type(PSymbolTable t);
  Symbol::PType result_type;
};

class NocleBinaryRange: public NocleBinaryOther{
public:
  NocleBinaryRange(): NocleBinaryOther(){};
  NocleBinaryRange(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL);
protected:
  virtual bool check_child_type(BaseSymbolType bst1, BaseSymbolType bst2);
};

class NocleBinaryPoint: public NocleBinaryOther{
public:
  NocleBinaryPoint(): NocleBinaryOther(){};
  NocleBinaryPoint(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL):
    NocleBinaryOther(lex, ch1, ch2, NULL){};
protected:
  virtual bool check_lvalue(PSymbolTable);
  // virtual Symbol::PType get_wanted_result_type(PSymbolTable t);
  virtual Symbol::PType get_result_type(PSymbolTable t);
  virtual bool check_child_type(BaseSymbolType bst1, BaseSymbolType bst2);
};

class NocleBinaryCompare: public NocleBinaryOther{
public:
  NocleBinaryCompare(): NocleBinaryOther(){};
  NocleBinaryCompare(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL):
    NocleBinaryOther(lex, ch1, ch2, NULL){};
protected:
  virtual Symbol::PType get_wanted_result_type(PSymbolTable t);
  virtual bool check_child_type(BaseSymbolType bst1, BaseSymbolType bst2);
};

class NocleArrayElement: public NocleBinaryOther{
public:
  NocleArrayElement(PNocle left, PNocle right): NocleBinaryOther(
    Lexeme("array_element", 0, 0, LT_ARRAY_ELEMENT), left, right){};
protected:
  virtual Symbol::PType get_wanted_result_type(PSymbolTable t);
  virtual bool check_child_type(BaseSymbolType bst1, BaseSymbolType bst2);
};

class NocleBinaryAssigment: public NocleBinary{
public:
  NocleBinaryAssigment(): NocleBinary(){};
  NocleBinaryAssigment(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL):
    NocleBinary(lex, ch1, ch2){};
  virtual bool check_types(PSymbolTable t);
};

////////////////////////////////////////////////////////////////////////////////
//NocleMulty

class NocleMulty: public Nocle{
public:
  NocleMulty(): Nocle(){};
  NocleMulty(Lexeme lex): Nocle(lex){};
  void add_child(PNocle ch);
  PNocle get_child(int ind);
  PNocle operator[](int ind);
  virtual Symbol::PType get_result_type(PSymbolTable t);
  virtual void get_str(std::stringstream& ss, int depth = 0);
protected:
  std::vector<PNocle> children;
};

class NocleFunction: public NocleMulty{
public:
  NocleFunction(): NocleMulty(Lexeme("function_call", 0, 0, LT_SEPARATOR)){};
  virtual Symbol::PType get_result_type(PSymbolTable t);
  virtual bool check_types(PSymbolTable t);
private:
  // static Lexeme func = Lexeme("function", 0, 0, LT_SEPARATOR);
};

class NocleBlock: public NocleMulty{
public:
  NocleBlock(): NocleMulty(Lexeme("block", 0, 0, LT_SEPARATOR)){};
  NocleBlock(PSymbolTable t): NocleMulty(Lexeme("block", 0, 0, LT_SEPARATOR)),
    table(t){};
  virtual bool check_types(PSymbolTable);
  void print_table();
private:
  PSymbolTable table;
  // static Lexeme func = Lexeme("function", 0, 0, LT_SEPARATOR);
};

class NocleIf: public NocleMulty{
public:
  NocleIf(): NocleMulty(){};
  NocleIf(PNocle expr, PNocle block_if, PNocle block_else): NocleMulty(Lexeme("if", 0, 0, LT_SEPARATOR)){
    this->add_child(expr);
    this->add_child(block_if);
    this->add_child(block_else);
  };
  bool check_types(PSymbolTable);
};

class NocleWhile: public NocleBinary{
public:
  NocleWhile(): NocleBinary(){};
  NocleWhile(PNocle expr, PNocle block): NocleBinary(Lexeme("while", 0, 0, LT_SEPARATOR),
    expr, block){};
  bool check_types(PSymbolTable);
};

class NocleFor: public NocleMulty{
public:
  NocleFor(): NocleMulty(){};
  NocleFor(PSymbolTable st, PNocle expr, PNocle block): NocleMulty(Lexeme("for", 0, 0, LT_SEPARATOR)), stable(st){
    this->add_child(expr);
    this->add_child(block);
  };
  bool check_types(PSymbolTable);
private:
  PSymbolTable stable;
};

class NocleArrayConst: public NocleMulty{
public:
  NocleArrayConst(): NocleMulty(Lexeme("const_array", 0, 0, LT_SEPARATOR)){};
};

// class NocleProgram: public Nocle{
// public:
//   NocleProgram(): NocleMulty(){};
//   void set_block();
//   void set_symbol_table();
//   PNocle get_block();
//   PSymbolTable get_symbol_table();
// }

typedef std::shared_ptr<NocleMulty> PNocleMulty;
typedef std::shared_ptr<NocleUnary> PNocleUnary;
typedef std::shared_ptr<NocleBinary> PNocleBinary;

////////////////////////////////////////////////////////////////////////////////
namespace Symbol{
  class Variable: public Base{
  public:
    Variable(){};
    Variable(std::string name, PType t);
    Variable(std::string name, PType t, PNocle init);
    PType get_type();
    virtual std::string get_type_name();
  protected:
    PType type;
    PNocle initialization;
  };

  class Reference: public Variable{
  public:
    Reference(){};
    Reference(std::string name, PType t): Variable(name, t){};
    Reference(std::string name, PType t, PNocle init): Variable(name, t, init){};
  private:
  };

  class Const: public Variable{
  public:
    Const(){};
    Const(std::string name, PType t, PNocle init): Variable(name, t, init){};
  private:
  };

  class Record: public Type{
  public:
    Record(){};
    Record(std::string name, PSymbolTable st):
      Type(name), stable(st){};
    PBase get_symbol(std::string name);
    PSymbolTable get_table();
    BaseSymbolType get_base_type();
  private:
    PSymbolTable stable;
  };

  class Procedure: public Base{
  public:
    Procedure();
    Procedure(std::string name, PSymbolTable st);
    void set_block(PNocle b);
    PType get_return_type();
    PSymbolTable get_table();
  protected:
    PSymbolTable stable;
    PNocle block;
  };

  class Function: public Procedure{
  public:
    Function(){};
    Function(std::string name, PSymbolTable st):
      Procedure(name, st){};

  private:
    // PSymbolTable stable;
    // PNocle block;
  };
  typedef std::shared_ptr<Procedure> PProcedure;
};
// typedef std::shared_ptr<NocleBinary> PNocleBinary;
// typedef std::shared_ptr<NocleBinary> PNocleBinary;

////////////////////////////////////////////////////////////////////////////////
