#pragma once

#include <defs.h>
#include <string>
#include <memory>

namespace Symbol{
  class Base;
  class Type;
  class Integer;
  class Float;
  class String;
  class Record;
  class Reference;
  class Array;
  class Alias;
  class Variable;
  class Const;

  typedef std::shared_ptr<Base> PBase;
  typedef std::shared_ptr<Type> PType;
  typedef std::shared_ptr<Variable> PVariable;

  class Base{
  public:
    std::string get_name();
    void set_name(std::string);
  private:
    std::string name;
  };

  class Type: public Base{
  public:
    Type();
    Type(std::string);
  };

  class Integer: public Type{

  };

  class Float: public Type{

  };

  class String: public Type{

  };

  class Reference: public Type{
  public:
    Reference();
    Reference(std::string name, PType t);
  private:
    PType type;
  };

  class Array: public Type{
  public:
    Array();
    Array(std::string name, PType type, int c);
  private:
    int count;
    PType type;
  };

  class Alias: public Type{
  public:
    Alias();
    Alias(std::string name, PType type);
  private:
    PType type;
  };

  class Variable: public Base{
  public:
    Variable();
    Variable(std::string name, PType t);
  private:
    PType type;
  };

  class Const: public Base{
  public:
    Const();
    Const(std::string name, PType type, Lexeme value);
  private:
    PType type;
    Lexeme value;
  };
};
