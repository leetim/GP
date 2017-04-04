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
  virtual NocleType get_type();
  virtual void get_str(std::stringstream& ss, int depth = 0);
  virtual bool check_lvalue(PSymbolTable);
  virtual bool check_types(PSymbolTable);
  // virtual Symbol::PType get_type();
private:
  Lexeme lexeme;
};
// typedef std::shared_ptr<Nocle> PNocle;
typedef std::shared_ptr<Nocle> PNocle;

class NocleTerminate: public Nocle{
public:
  NocleTerminate(): Nocle(){};
  NocleTerminate(Lexeme lex): Nocle(lex){};
  NocleType get_type();
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
  NocleType get_type();
  virtual void get_str(std::stringstream& ss, int depth = 0);
protected:
  std::vector<PNocle> children;
};

class NocleUnaryPrefix: public NocleUnary{
public:
  NocleUnaryPrefix(): NocleUnary(){};
  NocleUnaryPrefix(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
  NocleType get_type();
};

class NocleUnaryPostfix: public NocleUnary{
public:
  NocleUnaryPostfix(): NocleUnary(){};
  NocleUnaryPostfix(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
  NocleType get_type();
};

class NocleBinaryLeft: public NocleBinary{
public:
  NocleBinaryLeft(): NocleBinary(){};
  NocleBinaryLeft(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinary(lex, ch1, ch2){};
  NocleType get_type();
};

class NocleBinaryRight: public NocleBinary{
public:
  NocleBinaryRight(): NocleBinary(){};
  NocleBinaryRight(Lexeme lex, PNocle ch1 = NULL, PNocle ch2 = NULL): NocleBinary(lex, ch1, ch2){};
  NocleType get_type();
};

class NocleFunction: public NocleMulty{
public:
  NocleFunction(): NocleMulty(Lexeme("function", 0, 0, LT_SEPARATOR)){};
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
  private:
    PSymbolTable stable;
    PNocle block;
  };

  class Function: public Base{
  public:
    Function();
    Function(std::string name, PSymbolTable st, PNocle b);
  private:
    PSymbolTable stable;
    PNocle block;
  };
};

////////////////////////////////////////////////////////////////////////////////
