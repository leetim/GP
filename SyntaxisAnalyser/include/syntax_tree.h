#pragma once

#include <memory.h>
#include <defs.h>

class Nocle;
typedef std::shared_ptr<Nocle> PNocle;
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
  virtual NocleType get_type();
private:
  Lexeme lexeme;
};

class NocleTerminate: public Nocle{
public:
  NocleTerminate(): Nocle(){};
  NocleTerminate(Lexeme lex): Nocle(lex){};
  NocleType get_type();
};

class NocleUnary: public Nocle{
public:
  NocleUnary(): Nocle(){};
  NocleUnary(Lexeme lex, PNocle ch = NULL): Nocle(lex), child(ch){};
  void set_child(PNocle ch);
  PNocle get_child();
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
private:
  PNocle child1;
  PNocle child2;
};

class NocleExpression: public NocleUnary{
public:
  NocleExpression(): NocleUnary(){};
  NocleExpression(Lexeme lex, PNocle ch = NULL): NocleUnary(lex, ch){};
  NocleType get_type();
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

typedef std::shared_ptr<NocleExpression> PNocleExpression;
typedef std::shared_ptr<NocleUnaryPrefix> PNocleUnaryPrefix;
typedef std::shared_ptr<NocleUnaryPostfix> PNocleUnaryPostfix;
typedef std::shared_ptr<NocleBinaryLeft> PNocleBinaryLeft;
typedef std::shared_ptr<NocleBinaryRight> PNocleBinaryRight;

// class SyntaxBinary: public SyntaxTree{
//
// };
//
// class SyntaxUnary: public SyntaxTree{
//
// };
