#pragma once

#include <defs.h>
#include <string>
#include <memory>
enum BaseSymbolType{
  BST_NONE,
  BST_VOID,
  BST_INTEGER,
  BST_FLOAT,
  BST_STRING,
  BST_BOOL,
  BST_RANGE,
  BST_RECORD,
  BST_REFERENCE,
  BST_ARRAY,
  BST_ALIAS,
  BST_CLASS
};



namespace Symbol{
  class Base;
  class Type;
  class Void;
  class Integer;
  class Float;
  class String;
  class Bool;
  class Range;
  class Record;
  class Class;
  class Array;
  class Alias;
  class Variable;
  class Const;

  typedef std::shared_ptr<Base> PBase;
  typedef std::shared_ptr<Variable> PVariable;
  typedef std::shared_ptr<Type> PType;
  typedef std::shared_ptr<Void> PVoid;
  typedef std::shared_ptr<Integer> PInteger;
  typedef std::shared_ptr<Float> PFloat;
  typedef std::shared_ptr<String> PString;
  typedef std::shared_ptr<Bool> PBool;
  typedef std::shared_ptr<Range> PRange;
  typedef std::shared_ptr<Record> PRecord;
  typedef std::shared_ptr<Class> PClass;
  typedef std::shared_ptr<Array> PArray;
  typedef std::shared_ptr<Alias> PAlias;

  class Base{
  public:
    virtual std::string get_name();
    void set_name(std::string);
    virtual PType get_type();
    virtual std::string get_type_name();
  private:
    std::string name;
  };

  class Type: public Base{
  public:
    Type(): base_type(){};
    Type(std::string);
    Type(std::string, BaseSymbolType bst);
    BaseSymbolType get_base_type();
    PType get_type();
    bool is_casted_to(PType t);
    virtual bool compare(PType t1);
    virtual std::string get_type_name();
  private:
    BaseSymbolType base_type;
  };

  class Class: public Type{
  public:
    Class(): Type("Class", BST_CLASS){};
  };

  class Integer: public Type{
  public:
    Integer(): Type("Int", BST_INTEGER){};
  };

  class Void: public Type{
  public:
    Void(): Type("Void", BST_VOID){};
  };

  class Float: public Type{
  public:
    Float(): Type("Float", BST_FLOAT){};
  };

  class String: public Type{
  public:
    String(): Type("String", BST_STRING){};
  };

  class Bool: public Type{
  public:
    Bool(): Type("Bool", BST_BOOL){};
  };

  class Range: public Type{
  public:
    Range(): Type("Range", BST_RANGE){};
  };

  class Array: public Type{
  public:
    Array(PType t, int c): Type("Array", BST_ARRAY), type(t), count(c){};
    std::string get_name();
    bool compare(PType t1);
  private:
    PType type;
    int count;
  };

  class Alias: public Type{
  public:
    Alias(){};
    Alias(std::string name, PType t): Type(name, BST_ALIAS), type(t){};
    bool compare(PType t1);
  private:
    PType type;
  };
};
