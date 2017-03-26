#pragma once

#include <defs.h>
#include <string>
#include <sstream>
#include <memory>

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
  void get_str(std::stringstream& ss, int depth = 0);
private:
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

typedef std::shared_ptr<NocleMulty> PNocleMulty;
typedef std::shared_ptr<NocleUnaryPrefix> PNocleUnaryPrefix;
typedef std::shared_ptr<NocleUnaryPostfix> PNocleUnaryPostfix;
typedef std::shared_ptr<NocleBinaryLeft> PNocleBinaryLeft;
typedef std::shared_ptr<NocleBinaryRight> PNocleBinaryRight;
