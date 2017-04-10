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
  virtual Symbol::PType get_type(PSymbolTable t);
  virtual void get_str(std::stringstream& ss, int depth = 0);
  virtual bool check_lvalue(PSymbolTable);
  virtual bool check_types(PSymbolTable);
private:
  Lexeme lexeme;
};
// typedef std::shared_ptr<Nocle> PNocle;
typedef std::shared_ptr<Nocle> PNocle;

class NocleTerminate: public Nocle{
public:
  NocleTerminate(): Nocle(){};
  NocleTerminate(Lexeme lex): Nocle(lex){};
  Symbol::PType get_type(PSymbolTable t);
  virtual void get_str(std::stringstream& ss, int depth = 0);
};

class NocleUnary: public Nocle{
public:
  NocleUnary(): Nocle(){};
  NocleUnary(Lexeme lex, PNocle ch = NULL): Nocle(lex), child(ch){};
  void set_child(PNocle ch);
  PNocle get_child();
  void get_str(std::stringstream& ss, int depth = 0);
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

class NocleUnaryAddAdd: public NocleUnary{
public:
  NocleUnaryAddAdd(): NocleUnary(){};
  NocleUnaryAddAdd(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
};

class NocleUnarySubSub: public NocleUnary{
public:
  NocleUnarySubSub(): NocleUnary(){};
  NocleUnarySubSub(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
};

class NocleUnaryNot: public NocleUnary{
public:
  NocleUnaryNot(): NocleUnary(){};
  NocleUnaryNot(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
};

class NocleUnaryCast: public NocleUnary{
public:
  NocleUnaryCast(): NocleUnary(){};
  NocleUnaryCast(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
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

class NocleBinary: public Nocle{
public:
  NocleBinary(): Nocle(){};
  NocleBinary(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): Nocle(lex), child1(ch1), child2(ch2){};
  void set_child1(PNocle child);
  PNocle get_child1();
  void set_child2(PNocle child);
  PNocle get_child2();
  void get_str(std::stringstream& ss, int depth = 0);
private:
  PNocle child1;
  PNocle child2;
};

class NocleMulty: public Nocle{
public:
  NocleMulty(): Nocle(){};
  NocleMulty(Lexeme lex): Nocle(lex){};
  void add_child(PNocle ch);
  PNocle get_child(int ind);
  PNocle operator[](int ind);
  Symbol::PType get_type(PSymbolTable t);
  virtual void get_str(std::stringstream& ss, int depth = 0);
protected:
  std::vector<PNocle> children;
};

class NocleUnaryPrefix: public NocleUnary{
public:
  NocleUnaryPrefix(): NocleUnary(){};
  NocleUnaryPrefix(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
  Symbol::PType get_type(PSymbolTable t);
};

class NocleUnaryPostfix: public NocleUnary{
public:
  NocleUnaryPostfix(): NocleUnary(){};
  NocleUnaryPostfix(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
  Symbol::PType get_type(PSymbolTable t);
};

class NocleBinaryLeft: public NocleBinary{
public:
  NocleBinaryLeft(): NocleBinary(){};
  NocleBinaryLeft(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinary(lex, ch1, ch2){};
  Symbol::PType get_type(PSymbolTable t);
};

class NocleBinaryRight: public NocleBinary{
public:
  NocleBinaryRight(): NocleBinary(){};
  NocleBinaryRight(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinary(lex, ch1, ch2){};
  Symbol::PType get_type(PSymbolTable t);
};

class NocleBinaryAdd: public NocleBinaryLeft{
public:
  NocleBinaryAdd(): NocleBinaryLeft(){};
  NocleBinaryAdd(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinarySub: public NocleBinaryLeft{
public:
  NocleBinarySub(): NocleBinaryLeft(){};
  NocleBinarySub(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryMult: public NocleBinaryLeft{
public:
  NocleBinaryMult(): NocleBinaryLeft(){};
  NocleBinaryMult(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryDiv: public NocleBinaryLeft{
public:
  NocleBinaryDiv(): NocleBinaryLeft(){};
  NocleBinaryDiv(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryPower: public NocleBinaryLeft{
public:
  NocleBinaryPower(): NocleBinaryLeft(){};
  NocleBinaryPower(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryOr: public NocleBinaryLeft{
public:
  NocleBinaryOr(): NocleBinaryLeft(){};
  NocleBinaryOr(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryAnd: public NocleBinaryLeft{
public:
  NocleBinaryAnd(): NocleBinaryLeft(){};
  NocleBinaryAnd(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryXor: public NocleBinaryLeft{
public:
  NocleBinaryXor(): NocleBinaryLeft(){};
  NocleBinaryXor(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryMod: public NocleBinaryLeft{
public:
  NocleBinaryMod(): NocleBinaryLeft(){};
  NocleBinaryMod(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryShiftL: public NocleBinaryLeft{
public:
  NocleBinaryShiftL(): NocleBinaryLeft(){};
  NocleBinaryShiftL(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryShiftR: public NocleBinaryLeft{
public:
  NocleBinaryShiftR(): NocleBinaryLeft(){};
  NocleBinaryShiftR(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryLogicXor: public NocleBinaryLeft{
public:
  NocleBinaryLogicXor(): NocleBinaryLeft(){};
  NocleBinaryLogicXor(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryLogicOr: public NocleBinaryLeft{
public:
  NocleBinaryLogicOr(): NocleBinaryLeft(){};
  NocleBinaryLogicOr(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryLogicAnd: public NocleBinaryLeft{
public:
  NocleBinaryLogicAnd(): NocleBinaryLeft(){};
  NocleBinaryLogicAnd(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryEq: public NocleBinaryLeft{
public:
  NocleBinaryEq(): NocleBinaryLeft(){};
  NocleBinaryEq(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryNeq: public NocleBinaryLeft{
public:
  NocleBinaryNeq(): NocleBinaryLeft(){};
  NocleBinaryNeq(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryLt: public NocleBinaryLeft{
public:
  NocleBinaryLt(): NocleBinaryLeft(){};
  NocleBinaryLt(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryLtEq: public NocleBinaryLeft{
public:
  NocleBinaryLtEq(): NocleBinaryLeft(){};
  NocleBinaryLtEq(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryGt: public NocleBinaryLeft{
public:
  NocleBinaryGt(): NocleBinaryLeft(){};
  NocleBinaryGt(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryGtEq: public NocleBinaryLeft{
public:
  NocleBinaryGtEq(): NocleBinaryLeft(){};
  NocleBinaryGtEq(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryRange: public NocleBinaryLeft{
public:
  NocleBinaryRange(): NocleBinaryLeft(){};
  NocleBinaryRange(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleBinaryPoint: public NocleBinaryLeft{
public:
  NocleBinaryPoint(): NocleBinaryLeft(){};
  NocleBinaryPoint(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinaryLeft(lex, ch1, ch2){};
};

class NocleFunction: public NocleMulty{
public:
  NocleFunction(): NocleMulty(Lexeme("function_call", 0, 0, LT_SEPARATOR)){};
private:
  // static Lexeme func = Lexeme("function", 0, 0, LT_SEPARATOR);
};

class NocleArrayElement: public NocleBinaryRight{
public:
  NocleArrayElement(PNocle left, PNocle right): NocleBinaryRight(
    Lexeme("array_element", 0, 0, LT_SEPARATOR), left, right){};
private:
  // static Lexeme func = Lexeme("function", 0, 0, LT_SEPARATOR);
};

class NocleBlock: public NocleMulty{
public:
  NocleBlock(): NocleMulty(Lexeme("block", 0, 0, LT_SEPARATOR)){};
  NocleBlock(PSymbolTable t): NocleMulty(Lexeme("block", 0, 0, LT_SEPARATOR)),
    table(t){};
  bool check_types(PSymbolTable);
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
};

class NocleWhile: public NocleBinary{
public:
  NocleWhile(): NocleBinary(){};
  NocleWhile(PNocle expr, PNocle block): NocleBinary(Lexeme("while", 0, 0, LT_SEPARATOR),
    expr, block){};
};

class NocleFor: public NocleMulty{
public:
  NocleFor(): NocleMulty(){};
  NocleFor(PSymbolTable st, PNocle expr, PNocle block): NocleMulty(Lexeme("for", 0, 0, LT_SEPARATOR)), stable(st){
    this->add_child(expr);
    this->add_child(block);
  };
private:
  PSymbolTable stable;
};

class NocleArrayConst: public NocleMulty{
public:
  NocleArrayConst(): NocleMulty(Lexeme("const_array", 0, 0, LT_SEPARATOR)){};
};

typedef std::shared_ptr<NocleMulty> PNocleMulty;
typedef std::shared_ptr<NocleUnaryPrefix> PNocleUnaryPrefix;
typedef std::shared_ptr<NocleUnaryPostfix> PNocleUnaryPostfix;
typedef std::shared_ptr<NocleBinaryLeft> PNocleBinaryLeft;
typedef std::shared_ptr<NocleBinaryRight> PNocleBinaryRight;

////////////////////////////////////////////////////////////////////////////////
namespace Symbol{
  class Record: public Type{
  public:
    Record();
    Record(std::string name, PSymbolTable st);
  private:
    PSymbolTable stable;
  };

  class Procedure: public Base{
  public:
    Procedure();
    Procedure(std::string name, PSymbolTable st, PNocle b);
  protected:
    PSymbolTable stable;
    PNocle block;
  };

  class Function: public Procedure{
  public:
    Function(){};
    Function(std::string name, PSymbolTable st, PNocle b): Procedure(name, st, b){};
  private:
    // PSymbolTable stable;
    // PNocle block;
  };
};

////////////////////////////////////////////////////////////////////////////////
