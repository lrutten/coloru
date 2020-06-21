#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <deque>

#include "lex.h"

// exception klassen

class ZeroDivision
{
};

class ParserError
{
};

class Context;

class Element
{
public:
   virtual void show(int d) = 0;
   virtual Element *evaluate(Context *cx) = 0;
   Element();
   virtual ~Element()
   {
   }
};

class Number : public Element
{
public:
   explicit Number(number_t w);
   virtual ~Number();
   void show(int d) override; 
   Element *evaluate(Context *cx) override;
   number_t getNumber()
   {
      return number;
   }
   
private:
   number_t number;
};

class List : public Element
{
public:
   List();
   ~List();
   void add(Element *el);
   std::size_t size();
   Element *get(int i);
   std::deque<Element *> getElements()
   {
      return elements;
   }
   void pop_front()
   {
      elements.pop_front();
   }
   Element *evaluate(Context *cx) override;
   void show(int d) override;

private:
   std::deque<Element *> elements;   
};

class Vector : public Element
{
public:
   Vector();
   ~Vector();
   void add(Element *el);
   std::size_t size();
   Element *get(int i);
   std::vector<Element *> getElements()
   {
      return elements;
   }
   Element *evaluate(Context *cx) override;
   void show(int d) override;

private:
   std::vector<Element *> elements;   
};


class Body : public Element
{
public:
   Body();
   ~Body();
   void add(Element *el);
   std::size_t size();
   Element *get(int i);
   std::vector<Element *> getElements()
   {
      return elements;
   }
   Element *evaluate(Context *cx) override;
   void show(int d) override;

private:
   std::vector<Element *> elements;   
};


class Binary : public Element
{
public:
   Binary();
   virtual ~Binary();
   void add(Element *el);
   void show(int d) override;

protected:
   std::vector<Element *> elements;   
};

class Mul : public Binary
{
public:
   Mul()
   {
   }
   //virtual void druk(int d);
   void show(int d) override;
   Element *evaluate(Context *cx) override;
};

class Div : public Binary
{
public:
   Div()
   {
   }
   void show(int d) override;
   Element *evaluate(Context *cx) override;
};

class Plus : public Binary
{
public:
   Plus()
   {
   }
   void show(int d) override;
   Element *evaluate(Context *cx) override;
};

class Min : public Binary
{
public:
   Min()
   {
   }
   void show(int d) override;
   Element *evaluate(Context *cx) override;
};

class Equal : public Binary
{
public:
   Equal()
   {
   }
   void show(int d) override;
   Element *evaluate(Context *cx) override;
};

class Less : public Binary
{
public:
   Less()
   {
   }
   void show(int d) override;
   Element *evaluate(Context *cx) override;
};

class Greater : public Binary
{
public:
   Greater()
   {
   }
   void show(int d) override;
   Element *evaluate(Context *cx) override;
};

class NotEqual : public Binary
{
public:
   NotEqual()
   {
   }
   void show(int d) override;
   Element *evaluate(Context *cx) override;
};

class GreaterEq : public Binary
{
public:
   GreaterEq()
   {
   }
   void show(int d) override;
   Element *evaluate(Context *cx) override;
};

class LessEq : public Binary
{
public:
   LessEq()
   {
   }
   void show(int d) override;
   Element *evaluate(Context *cx) override;
};



class If : public Element
{
public:
   If();
   ~If();
   void show(int d) override;
   Element *evaluate(Context *cx) override;
   void setCondition(Element *cond)
   {
      condition = cond;
   }
   void setYes(Element *ys)
   {
      yes = ys;
   }
   void setNo(Element *n)
   {
      no = n;
   }
private:
   Element *condition;
   Element *yes;
   Element *no;
};

class Fn : public Element
{
public:
   Fn();
   ~Fn();
   void show(int d) override;
   Element *evaluate(Context *cx) override;
   void addParam(std::string param)
   {
      params.push_back(param);
   }
   void setBody(Body *bd)
   {
      body = bd;
   }
   
private:
   std::vector<std::string> params;
   Body                    *body;
};

class Defn : public Element
{
public:
   Defn();
   ~Defn();
   void show(int d) override;
   Element *evaluate(Context *cx) override;
   std::string getName()
   {
      return name;
   }
   void setName(const std::string &nm)
   {
      name = nm;
   }
   void setFn(Fn *f)
   {
      fn = f;
   }

private:
   std::string  name;
   Fn          *fn;
};

class Symbol : public Element
{
public:
   explicit Symbol(const std::string &te);
   ~Symbol();
   void show(int d) override;
   Element *evaluate(Context *cx) override;
   std::string getText()
   {
      return text;
   }
   
private:
   std::string text;
};

class Main : public Element
{
public:
   Main();
   virtual ~Main();
   void add(Element *el);
   void show(int d) override;
   Element *evaluate(Context *cx) override;

private:
   std::vector<Element *> elements;   
};


class Parser
{
private:
   Lex *lex;
   
public:
   Parser();
   ~Parser();
   Element *parse(std::string fn);
   Element *list(bool isliteral=false);
   Element *vector();
   Element *expression(bool isliteral=false);
   Element *main();
};

#endif

