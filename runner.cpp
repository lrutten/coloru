#include <iostream>

#include "runner.h"

// Number

Element_p Number::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(number);
}

// List

Element_p List::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Vector

Element_p Vector::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Body

Element_p Body::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Mul

Element_p Mul::evaluate(std::weak_ptr<Context> cx)
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

Element_p Div::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Plus

Element_p Plus::evaluate(std::weak_ptr<Context> cx)
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

Element_p Min::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Equal

Element_p Equal::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Less

Element_p Less::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Greater

Element_p Greater::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// NotEqual

Element_p NotEqual::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// GreaterEq

Element_p GreaterEq::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// LessEq

Element_p LessEq::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Defn

Element_p Defn::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Fn

Element_p Fn::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// If

Element_p If::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}

// Symbol

Element_p Symbol::evaluate(std::weak_ptr<Context> cx)
{
   return std::make_shared<Number>(0);
}


// Main

Element_p Main::evaluate(std::weak_ptr<Context> cx)
{
   Element_p result;
   for (Element_p el: elements)
   {
      result = el->evaluate(cx);
   }
   return result;
}

// Runner

Runner::Runner(Element_p rt) : root(rt)
{
}

Element_p Runner::run()
{
   std::cout << "start runner\n";
   std::shared_ptr<Context> cx = std::make_shared<Context>();
   Element_p rs = root->evaluate(cx);
   return rs;
}
