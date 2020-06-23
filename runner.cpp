#include <iostream>

#include "runner.h"

// Number

Element_p Number::evaluate(std::shared_ptr<Context> cx)
{
   return std::make_shared<Number>(number);
}

// Boolean

Element_p Boolean::evaluate(std::shared_ptr<Context> cx)
{
   return shared_from_this();
}


// List

Element_p List::evaluate(std::shared_ptr<Context> cx)
{
   std::cout << "List evaluate()\n";
   Element_p  el = get(0);
   Callable_p ca = std::dynamic_pointer_cast<Callable>(el);
   if (ca != nullptr)
   {
      Bind_p   bi  = std::dynamic_pointer_cast<Bind>(ca);
      Fn_p     fuu = std::dynamic_pointer_cast<Fn>(ca);
      Symbol_p sy  = std::dynamic_pointer_cast<Symbol>(ca);
      if (sy != nullptr)
      {
         Element_p fu = cx->search(sy->getText());
         if (fu == nullptr)
         {
            std::cout << "function not found\n";
            throw std::make_shared<RunError>();
         }
         std::cout << "name found\n";
         fuu = std::dynamic_pointer_cast<Fn>(fu);
         if (fuu == nullptr)
         {
            std::cout << "name found but is not a function\n";
            throw std::make_shared<RunError>();
         }
      }
      else
      if (bi != nullptr)
      {
         fuu = bi->getFn();
      }

      // handle the parameters
      int fparsize = fuu->getParamsSize();
      int aparsize = size() - 1;
      std::cout << "fparsize " << fparsize << " aparsize " << fparsize << "\n";
      
      if (aparsize == fparsize)
      {
         Frame_p fr = std::make_shared<Frame>();
         
         for (int i=0; i<fparsize; i++)
         {
            Element_p apar = get(i + 1);
            std::string fparname = fuu->getParam(i);
            Element_p aparres = apar->evaluate(cx);
            std::cout << "==== aparam result===\n";
            aparres->show(0);
            std::cout << "=====\n";
            fr->add_binding(fparname, aparres);
         }
         cx->push(fr);
         Element_p rs = fuu->evaluate(cx);
         cx->pop();
         
         std::cout << "++++ fn result+++\n";
         rs->show(0);
         std::cout << "+++++++++\n";
         return rs;
      }
      else
      {
         std::cout << "number of actual and formal parameters is different\n";
         throw std::make_shared<RunError>();
      }
   }
   else
   {
      std::cout << "the first element of a list must be a symbol or a function\n";
      throw std::make_shared<RunError>();
   }
   return std::make_shared<Number>(0);
}
 
 
// Vector

Element_p Vector::evaluate(std::shared_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Body

Element_p Body::evaluate(std::shared_ptr<Context> cx)
{
   std::cout << "Body evaluate\n";
   Element_p result;
   for (Element_p el: elements)
   {
      result = el->evaluate(cx);
   }
   return result;
}

// Mul

Element_p Mul::evaluate(std::shared_ptr<Context> cx)
{
   number_t value = 1;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx);
      Number_p nu = std::dynamic_pointer_cast<Number>(el2);
      if (nu == nullptr)
      {
         std::cout << "run error\n";
         throw std::make_unique<RunError>();
      }
      value *= nu->getNumber();
   }
   return std::make_shared<Number>(value);
}


// Div

Element_p Div::evaluate(std::shared_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Plus

Element_p Plus::evaluate(std::shared_ptr<Context> cx)
{
   number_t value = 0;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx);
      Number_p nu = std::dynamic_pointer_cast<Number>(el2);
      if (nu == nullptr)
      {
         std::cout << "run error\n";
         throw std::make_unique<RunError>();
      }
      value += nu->getNumber();
   }
   return std::make_shared<Number>(value);
}


// Min

Element_p Min::evaluate(std::shared_ptr<Context> cx)
{
   number_t value = 0;
   int i = 0;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx);
      Number_p nu = std::dynamic_pointer_cast<Number>(el2);
      if (nu == nullptr)
      {
         std::cout << "run error\n";
         throw std::make_unique<RunError>();
      }
      if (i == 0)
      {
         value += nu->getNumber();
      }
      else
      {
         value -= nu->getNumber();
      }
      i++;
   }
   return std::make_shared<Number>(value);
}

// Equal

Element_p Equal::evaluate(std::shared_ptr<Context> cx)
{
   bool bo = true;
   Number_p last = nullptr;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx);
      Number_p nu = std::dynamic_pointer_cast<Number>(el2);
      if (nu == nullptr)
      {
         std::cout << "only numbers can be compared\n";
         throw std::make_unique<RunError>();
      }
      if (last != nullptr && last->getNumber() != nu->getNumber())
      {
         return std::make_shared<Boolean>(false);
      }
      last = nu;
   }
   return std::make_shared<Boolean>(true);
}

// Less

Element_p Less::evaluate(std::shared_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Greater

Element_p Greater::evaluate(std::shared_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// NotEqual

Element_p NotEqual::evaluate(std::shared_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// GreaterEq

Element_p GreaterEq::evaluate(std::shared_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// LessEq

Element_p LessEq::evaluate(std::shared_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Defn

Element_p Defn::evaluate(std::shared_ptr<Context> cx)
{
   cx->add_binding(name, fn);
   
   // return waarde nog niet ok
   return shared_from_this();
}

// Fn

Element_p Fn::evaluate(std::shared_ptr<Context> cx)
{
   std::cout << "Fn evaluate\n";
   return body->evaluate(cx);
}

// Bind

Element_p Bind::evaluate(std::shared_ptr<Context> cx)
{
   std::cout << "Bind evaluate\n";
   return fn->evaluate(cx);
}

// If

Element_p If::evaluate(std::shared_ptr<Context> cx)
{
   std::cout << "If evaluate\n";
   Element_p el = condition->evaluate(cx);
   Boolean_p bo = std::dynamic_pointer_cast<Boolean>(el);
   if (bo == nullptr)
   {
      std::cout << "no boolean expression\n";
      throw std::make_unique<RunError>();
   }
   
   if (bo->getValue())
   {
      std::cout << "If yes\n";
      return yes->evaluate(cx);
   }
   else
   if (no != nullptr)
   {
      std::cout << "If no\n";
      return no->evaluate(cx);
   }
   else
   {
      // should be nil
      return std::make_shared<Number>(0);
   }
}

// Symbol

Element_p Symbol::evaluate(std::shared_ptr<Context> cx)
{
   Element_p val = cx->search(text);
   if (val == nullptr)
   {
      std::cout << "variable " << text << " not found\n";
      throw std::make_shared<RunError>();
   }
   else
   {
      return val;
   }
}


// Main

Element_p Main::evaluate(std::shared_ptr<Context> cx)
{
   Element_p result;
   for (Element_p el: elements)
   {
      result = el->evaluate(cx);
   }
   return result;
}


// Frame

Frame::Frame()
{
}

Frame::~Frame()
{
}

void Frame::add_binding(std::string nm, Element_p el)
{
   bindings.insert({nm, el});
}

Element_p Frame::search(std::string nm)
{
   std::cout << "Frame search\n";
   auto it = bindings.find(nm);
   if (it != bindings.end())
   {
      return it->second;
   }
   else
   {
      return nullptr;
   }
}

// Context

Context::Context()
{
}

Context::~Context()
{
}

void Context::push()
{
   frames.push_front(std::make_shared<Frame>());
}

void Context::push(Frame_p fr)
{
   frames.push_front(fr);
}

void Context::pop()
{
   frames.pop_front();
}

void Context::add_binding(std::string nm, Element_p el)
{
   frames.front()->add_binding(nm, el);
}

Element_p Context::search(std::string nm)
{
   std::cout << "Context search\n";
   for (Frame_p fra: frames)
   {
      Element_p el = fra->search(nm);
      if (el != nullptr)
      {
         return el;
      }
   }
   return nullptr;
   //return frames.front()->search(nm);
}

// Runner

Runner::Runner(Element_p rt) : root(rt)
{
}

Element_p Runner::run()
{
   std::cout << "start runner\n";
   Context_p cx = std::make_shared<Context>();
   cx->push();
   Element_p rs = root->evaluate(cx);
   cx->pop();
   return rs;
}
