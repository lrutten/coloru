#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <deque>
#include <map>
#include <memory>


#include "lex.h"

extern void indent(int d);
extern bool debug;

// exception classes

class ZeroDivision
{
};

class ParserError
{
};

class Frame;
class Context;


// Element

class Element : public std::enable_shared_from_this<Element>
{
public:
   Element();
   virtual ~Element()
   {
   }
   virtual void show(int d) = 0;
   virtual std::shared_ptr<Element> evaluate(std::shared_ptr<Context> cx, int d) = 0;
   virtual std::shared_ptr<Element> capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d)
   {
      return shared_from_this();
   }
   virtual std::string info()
   {
      return "Element";
   }
   virtual void print()
   {
   }
};

using Element_p = std::shared_ptr<Element>;


// Callable

class Callable : public Element
{
public:
   Callable()
   {
   }
   ~Callable()
   {
   }
};

using Callable_p = std::shared_ptr<Callable>;

// Number

class Number : public Element
{
public:
   explicit Number(number_t w);
   virtual ~Number();
   void show(int d) override; 
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   number_t getNumber()
   {
      return number;
   }
   virtual std::string info() override
   {
      return "Number";
   }
   void print() override
   {
      std::cout << number;
   }
   
private:
   number_t number;
};

using Number_p = std::shared_ptr<Number>;


// Boolean

class Boolean : public Element
{
public:
   explicit Boolean(bool w);
   virtual ~Boolean();
   void show(int d) override; 
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   number_t getValue()
   {
      return value;
   }
   virtual std::string info() override
   {
      return "Boolean";
   }
   void print() override
   {
      if (value)
      {
         std::cout << true;
      }
      else
      {
         std::cout << false;
      }
   }
   
private:
   bool value;
};

using Boolean_p = std::shared_ptr<Boolean>;


// Nil

class Nil : public Element
{
public:
   explicit Nil();
   virtual ~Nil();
   void show(int d) override; 
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   virtual std::string info() override
   {
      return "Nil";
   }
   void print() override
   {
      std::cout << "nil";
   }
   
private:
};

using Nil_p = std::shared_ptr<Nil>;


// List

class List : public Element
{
public:
   List();
   ~List();
   void add(Element_p el);
   std::size_t size();
   Element_p get(int i);
   std::deque<Element_p > getElements()
   {
      return elements;
   }
   void pop_front()
   {
      elements.pop_front();
   }
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   void show(int d) override;
   virtual std::string info() override
   {
      return "List";
   }
   void print() override;

private:
   std::deque<Element_p> elements;   
};

using List_p = std::shared_ptr<List>;


// Call

class Call : public Element
{
public:
   Call();
   ~Call();
   void add(Element_p el);
   std::size_t size();
   Element_p get(int i);
   std::deque<Element_p > getElements()
   {
      return elements;
   }
   void pop_front()
   {
      elements.pop_front();
   }
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   void show(int d) override;
   virtual std::string info() override
   {
      return "Call";
   }

private:
   std::deque<Element_p> elements;   
};

using Call_p = std::shared_ptr<Call>;


// Elements

class Elements : public Element
{
public:
   Elements();
   ~Elements();
   void add(Element_p el);
   std::size_t size();
   Element_p get(int i);
   std::vector<Element_p > getElements()
   {
      return elements;
   }
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   virtual std::shared_ptr<Elements> make_copy() = 0;

protected:
   std::vector<Element_p> elements;   
};

using Elements_p = std::shared_ptr<Elements>;


// Vector

class Vector : public Elements
{
public:
   Vector();
   ~Vector();
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   void show(int d) override;
   Elements_p make_copy() override;
   virtual std::string info() override
   {
      return "Vector";
   }

private:
};

using Vector_p = std::shared_ptr<Vector>;


class Body : public Elements
{
public:
   Body();
   ~Body();
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   void show(int d) override;
   Elements_p make_copy() override;
   virtual std::string info() override
   {
      return "Body";
   }

private:
};

using Body_p = std::shared_ptr<Body>;


class Binary : public Elements
{
public:
   Binary();
   virtual ~Binary();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Elements_p make_copy() override
   {
      return nullptr;
   }

protected:
};

using Binary_p = std::shared_ptr<Binary>;

class Mul : public Binary
{
public:
   Mul()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Elements_p make_copy() override
   {
      return std::make_shared<Mul>();
   }
   virtual std::string info() override
   {
      return "Mul";
   }
};

class Div : public Binary
{
public:
   Div()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Elements_p make_copy() override
   {
      return std::make_shared<Div>();
   }
   virtual std::string info() override
   {
      return "Div";
   }
};

class Plus : public Binary
{
public:
   Plus()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Elements_p make_copy() override
   {
      return std::make_shared<Plus>();
   }
   virtual std::string info() override
   {
      return "Plus";
   }
};

class Min : public Binary
{
public:
   Min()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Elements_p make_copy() override
   {
      return std::make_shared<Min>();
   }
   virtual std::string info() override
   {
      return "Min";
   }
};

class Equal : public Binary
{
public:
   Equal()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Elements_p make_copy() override
   {
      return std::make_shared<Equal>();
   }
   virtual std::string info() override
   {
      return "Equal";
   }
};

class Less : public Binary
{
public:
   Less()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Elements_p make_copy() override
   {
      return std::make_shared<Less>();
   }
   virtual std::string info() override
   {
      return "Less";
   }
};

class Greater : public Binary
{
public:
   Greater()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Elements_p make_copy() override
   {
      return std::make_shared<Greater>();
   }
   virtual std::string info() override
   {
      return "Greater";
   }
};

class NotEqual : public Binary
{
public:
   NotEqual()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Elements_p make_copy() override
   {
      return std::make_shared<NotEqual>();
   }
   virtual std::string info() override
   {
      return "NotEqual";
   }
};

class GreaterEq : public Binary
{
public:
   GreaterEq()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Elements_p make_copy() override
   {
      return std::make_shared<GreaterEq>();
   }
   virtual std::string info() override
   {
      return "GreaterEq";
   }
};

class LessEq : public Binary
{
public:
   LessEq()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Elements_p make_copy() override
   {
      return std::make_shared<LessEq>();
   }
   virtual std::string info() override
   {
      return "LessEq";
   }
};



class If : public Element
{
public:
   If();
   ~If();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   void setCondition(Element_p cond)
   {
      condition = cond;
   }
   void setYes(Element_p ys)
   {
      yes = ys;
   }
   void setNo(Element_p n)
   {
      no = n;
   }
   virtual std::string info() override
   {
      return "If";
   }

private:
   Element_p condition;
   Element_p yes;
   Element_p no;
};

using If_p = std::shared_ptr<If>;


// Println

class Println : public Element
{
public:
   Println();
   ~Println();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   void setBody(Body_p bd)
   {
      body = bd;
   }
   bool isFull()
   {
      return full;
   }
   void setFull(bool fu)
   {
      full = fu;
   }
   virtual std::string info() override
   {
      return "Println";
   }
   
private:
   bool                     full;
   Body_p                   body;
};

using Println_p = std::shared_ptr<Println>;

// Ampersand

class Ampersand : public Element
{
public:
   Ampersand();
   ~Ampersand();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d)
   {
   }
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d)
   {
   }
   virtual std::string info() override
   {
      return "Ampersand";
   }
   
private:
};

using Ampersand_p = std::shared_ptr<Ampersand>;


// Let

class Let : public Element
{
public:
   Let();
   ~Let();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   void addVariable(std::string nm, Element_p el)
   {
      variables.insert({nm, el});
   }
   int getVariablesSize()
   {
      return variables.size();
   }
   Element_p getVariable(std::string nm)
   {
      return variables[nm];
   }
   void setBody(Body_p bd)
   {
      body = bd;
   }
   bool isFull()
   {
      return full;
   }
   void setFull(bool fu)
   {
      full = fu;
   }
   virtual std::string info() override
   {
      return "Let";
   }
   
private:
   bool                             full;
   std::map<std::string, Element_p> variables;
   Body_p                           body;
};

using Let_p = std::shared_ptr<Let>;

class AParam
{
public:
   AParam();
   virtual ~AParam();
   virtual void show(int d) = 0;
   
   bool getRest()
   {
      return rest;
   }
   void setRest(bool rst)
   {
      rest = rst;
   }
   virtual bool assignParameters(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, List_p apars, int d, bool single=true) = 0;

protected:
   bool rest;    // preceding ampersand detected
};

using AParam_p = std::shared_ptr<AParam>;

class Param : public AParam
{
public:
   Param(std::string nm);
   ~Param();
   
   std::string getName()
   {
      return name;
   }
   void show(int d) override;
   bool assignParameters(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, List_p apars, int d, bool single=true) override;

private:
   std::string name;
};

using Param_p = std::shared_ptr<Param>;

class ParamList : public AParam
{
public:
   ParamList();
   ~ParamList();
   void addParam(AParam_p pa)
   {
      params.push_back(pa);
   }
   
   int size()
   {
      return params.size();
   }
   
   std::string get(int i)
   {
      AParam_p apar = params[i];
      Param_p par = std::dynamic_pointer_cast<Param>(apar);
      if (par != nullptr)
      {
         return par->getName();
      }
      else
      {
         return "";
      }
   }

   void show(int d) override;
   bool assignParameters(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, List_p apars, int d, bool single=true) override;
   
private:
   std::vector<AParam_p> params;
};

using ParamList_p = std::shared_ptr<ParamList>;

// Fn

class Fn : public Element
{
public:
   Fn();
   ~Fn();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   
   int getParamsSize()
   {
      return paramlist->size();
   }
   
   std::string getParam(int i)
   {
      return paramlist->get(i);
   }
   
   void setParamList(ParamList_p pl)
   {
      paramlist = pl;
   }

   void setBody(Body_p bd)
   {
      body = bd;
   }
   bool isFull()
   {
      return full;
   }
   void setFull(bool fu)
   {
      full = fu;
   }
   virtual std::string info() override
   {
      return "Fn";
   }
   
   bool assignParameters(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, Element_p call, int d);

private:
   bool                     full;
   ParamList_p              paramlist;
   Body_p                   body;
};

using Fn_p = std::shared_ptr<Fn>;

// Defn

class Defn : public Element
{
public:
   Defn();
   ~Defn();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   std::string getName()
   {
      return name;
   }
   void setName(const std::string &nm)
   {
      name = nm;
   }
   Fn_p getFn()
   {
      return fn;
   }
   void setFn(Fn_p f)
   {
      fn = f;
   }
   virtual std::string info() override
   {
      return "Defn";
   }

private:
   std::string  name;
   Fn_p         fn;
};

using Defn_p = std::shared_ptr<Defn>;

// Lambda

class Lambda : public Callable
{
public:
   Lambda();
   ~Lambda();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   Fn_p getFn()
   {
      return fn;
   }
   void setFn(Fn_p f)
   {
      fn = f;
   }
   virtual std::string info() override
   {
      return "Lambda";
   }

private:
   Fn_p         fn;
};

using Lambda_p = std::shared_ptr<Lambda>;

// Bind

class Frame;

class Bind : public Callable
{
public:
   Bind();
   ~Bind();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Lambda_p getLambda()
   {
      return lambda;
   }
   void setLambda(Lambda_p la)
   {
      lambda = la;
   }
   std::shared_ptr<Frame> getFrame()
   {
      return frame;
   }
   void setFrame(std::shared_ptr<Frame> fr)
   {
      frame = fr;
   }
   virtual std::string info() override
   {
      return "Bind";
   }
   
private:
   Lambda_p               lambda;
   std::shared_ptr<Frame> frame;
};

using Bind_p = std::shared_ptr<Bind>;

// Symbol

class Symbol : public Callable
{
public:
   explicit Symbol(const std::string &te);
   ~Symbol();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   std::string getText()
   {
      return text;
   }
   virtual std::string info() override
   {
      return "Symbol";
   }
   void print() override
   {
      std::cout << text;
   }
   
private:
   std::string text;
};

using Symbol_p = std::shared_ptr<Symbol>;

// Builtin

class Builtin : public Callable
{
public:
   explicit Builtin(const std::string &te);
   ~Builtin();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Element_p evaluate2(std::shared_ptr<Context> cx, std::shared_ptr<Element> call, int d);
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   std::string getText()
   {
      return text;
   }
   virtual std::string info() override
   {
      return "Builtin";
   }
   void print() override
   {
      std::cout << text;
   }
   
private:
   std::string text;
};

using Builtin_p = std::shared_ptr<Builtin>;

// Text

class Text : public Element
{
public:
   explicit Text(const std::string &te);
   ~Text();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   std::string getText()
   {
      return text;
   }
   virtual std::string info() override
   {
      return "Text";
   }
   void print() override
   {
      std::cout << text;
   }
   
private:
   std::string text;
};

using Text_p = std::shared_ptr<Text>;


class Main : public Elements
{
public:
   Main();
   virtual ~Main();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   std::shared_ptr<Elements> make_copy() override
   {
      return std::make_shared<Binary>();
   }
   virtual std::string info() override
   {
      return "Main";
   }

private:
};

using Main_p = std::shared_ptr<Main>;


class Parser
{
private:
   Lex_p lex;
   
public:
   Parser();
   ~Parser();
   Element_p parse(std::string fn);
   ParamList_p parameters(Element_p els);
   Element_p list(bool isliteral=false);
   Element_p vector();
   Element_p expression(bool isliteral=false);
   Element_p main();
};

using Parser_p = std::shared_ptr<Parser>;

#endif

