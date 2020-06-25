#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <deque>
#include <memory>


#include "lex.h"

extern void indent(int d);
extern bool debug;

// exception klassen

class ZeroDivision
{
};

class ParserError
{
};

class Frame;
class Context;

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
};

using Element_p = std::shared_ptr<Element>;

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
   
private:
   number_t number;
};

using Number_p = std::shared_ptr<Number>;


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
   
private:
   bool value;
};

using Boolean_p = std::shared_ptr<Boolean>;


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

private:
   std::deque<Element_p> elements;   
};

using List_p = std::shared_ptr<List>;


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


class Fn : public Element
{
public:
   Fn();
   ~Fn();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx, int d) override;
   Element_p capture(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, int d) override;
   void addParam(std::string param)
   {
      params.push_back(param);
   }
   int getParamsSize()
   {
      return params.size();
   }
   std::string getParam(int i)
   {
      return params[i];
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
   
private:
   bool                     full;
   std::vector<std::string> params;
   Body_p                   body;
};

using Fn_p = std::shared_ptr<Fn>;


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
   
private:
   std::string text;
};

using Symbol_p = std::shared_ptr<Symbol>;


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
   Element_p list(bool isliteral=false);
   Element_p vector();
   Element_p expression(bool isliteral=false);
   Element_p main();
};

using Parser_p = std::shared_ptr<Parser>;

#endif

