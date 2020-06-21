#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <deque>
#include <memory>


#include "lex.h"

// exception klassen

class ZeroDivision
{
};

class ParserError
{
};

class Context;

class Element : public std::enable_shared_from_this<Element>
{
public:
   virtual void show(int d) = 0;
   virtual std::shared_ptr<Element> evaluate(std::shared_ptr<Context> cx) = 0;
   Element();
   virtual ~Element()
   {
   }
};

using Element_p = std::shared_ptr<Element>;


class Number : public Element
{
public:
   explicit Number(number_t w);
   virtual ~Number();
   void show(int d) override; 
   Element_p evaluate(std::shared_ptr<Context> cx) override;
   number_t getNumber()
   {
      return number;
   }
   
private:
   number_t number;
};

using Number_p = std::shared_ptr<Number>;


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
   Element_p evaluate(std::shared_ptr<Context> cx) override;
   void show(int d) override;

private:
   std::deque<Element_p> elements;   
};

using List_p = std::shared_ptr<List>;


class Vector : public Element
{
public:
   Vector();
   ~Vector();
   void add(Element_p el);
   std::size_t size();
   Element_p get(int i);
   std::vector<Element_p > getElements()
   {
      return elements;
   }
   Element_p evaluate(std::shared_ptr<Context> cx) override;
   void show(int d) override;

private:
   std::vector<Element_p> elements;   
};

using Vector_p = std::shared_ptr<Vector>;


class Body : public Element
{
public:
   Body();
   ~Body();
   void add(Element_p el);
   std::size_t size();
   Element_p get(int i);
   std::vector<Element_p > getElements()
   {
      return elements;
   }
   Element_p evaluate(std::shared_ptr<Context> cx) override;
   void show(int d) override;

private:
   std::vector<Element_p> elements;   
};

using Body_p = std::shared_ptr<Body>;


class Binary : public Element
{
public:
   Binary();
   virtual ~Binary();
   void add(Element_p el);
   void show(int d) override;

protected:
   std::vector<Element_p > elements;   
};

using Binary_p = std::shared_ptr<Binary>;

class Mul : public Binary
{
public:
   Mul()
   {
   }
   //virtual void druk(int d);
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;
};

class Div : public Binary
{
public:
   Div()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;
};

class Plus : public Binary
{
public:
   Plus()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;
};

class Min : public Binary
{
public:
   Min()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;
};

class Equal : public Binary
{
public:
   Equal()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;
};

class Less : public Binary
{
public:
   Less()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;
};

class Greater : public Binary
{
public:
   Greater()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;
};

class NotEqual : public Binary
{
public:
   NotEqual()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;
};

class GreaterEq : public Binary
{
public:
   GreaterEq()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;
};

class LessEq : public Binary
{
public:
   LessEq()
   {
   }
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;
};



class If : public Element
{
public:
   If();
   ~If();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;
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
   Element_p evaluate(std::shared_ptr<Context> cx) override;
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
   
private:
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
   Element_p evaluate(std::shared_ptr<Context> cx) override;
   std::string getName()
   {
      return name;
   }
   void setName(const std::string &nm)
   {
      name = nm;
   }
   void setFn(Fn_p f)
   {
      fn = f;
   }

private:
   std::string  name;
   Fn_p         fn;
};

using Defn_p = std::shared_ptr<Defn>;


class Symbol : public Element
{
public:
   explicit Symbol(const std::string &te);
   ~Symbol();
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;
   std::string getText()
   {
      return text;
   }
   
private:
   std::string text;
};

using Symbol_p = std::shared_ptr<Symbol>;


class Main : public Element
{
public:
   Main();
   virtual ~Main();
   void add(Element_p el);
   void show(int d) override;
   Element_p evaluate(std::shared_ptr<Context> cx) override;

private:
   std::vector<Element_p> elements;   
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

