#include <iostream>

#include "runner.h"

// Number

Element *Number::evaluate(Context *cx)
{
   return new Number(number);
}

// List

Element *List::evaluate(Context *cx)
{
   return new Number(0);
}

// Vector

Element *Vector::evaluate(Context *cx)
{
   return new Number(0);
}

// Body

Element *Body::evaluate(Context *cx)
{
   return new Number(0);
}

// Mul

Element *Mul::evaluate(Context *cx)
{
   number_t value = 1;
   for (Element *el: elements)
   {
      Element *el2 = el->evaluate(cx);
      Number *nu = dynamic_cast<Number *>(el2);
      if (nu == nullptr)
      {
         std::cout << "run error\n";
         throw new RunError();
      }
      value *= nu->getNumber();
      delete nu;
   }
   return new Number(value);
}


// Div

Element *Div::evaluate(Context *cx)
{
   return new Number(0);
}

// Plus

Element *Plus::evaluate(Context *cx)
{
   number_t value = 0;
   for (Element *el: elements)
   {
      Element *el2 = el->evaluate(cx);
      Number *nu = dynamic_cast<Number *>(el2);
      if (nu == nullptr)
      {
         std::cout << "run error\n";
         throw new RunError();
      }
      value += nu->getNumber();
      delete nu;
   }
   return new Number(value);
}


// Min

Element *Min::evaluate(Context *cx)
{
   return new Number(0);
}

// Equal

Element *Equal::evaluate(Context *cx)
{
   return new Number(0);
}

// Less

Element *Less::evaluate(Context *cx)
{
   return new Number(0);
}

// Greater

Element *Greater::evaluate(Context *cx)
{
   return new Number(0);
}

// NotEqual

Element *NotEqual::evaluate(Context *cx)
{
   return new Number(0);
}

// GreaterEq

Element *GreaterEq::evaluate(Context *cx)
{
   return new Number(0);
}

// LessEq

Element *LessEq::evaluate(Context *cx)
{
   return new Number(0);
}

// Defn

Element *Defn::evaluate(Context *cx)
{
   return new Number(0);
}

// Fn

Element *Fn::evaluate(Context *cx)
{
   return new Number(0);
}

// If

Element *If::evaluate(Context *cx)
{
   return new Number(0);
}

// Symbol

Element *Symbol::evaluate(Context *cx)
{
   return new Number(0);
}


// Main

Element *Main::evaluate(Context *cx)
{
   Element *previous = nullptr;
   Element *result;
   for (Element *el: elements)
   {
      if (previous != nullptr)
      {
         delete previous;
         previous = nullptr;
      }
      result = el->evaluate(cx);
      previous = result;
   }
   return result;
}

// Runner

Runner::Runner(Element *rt) : root(rt)
{
}

Element *Runner::run()
{
   std::cout << "start runner\n";
   Context *cx = new Context();
   Element *rs = root->evaluate(cx);
   delete cx;
   return rs;
}
