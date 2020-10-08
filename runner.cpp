#include <iostream>

#include "runner.h"

// --------------- evaluate() ------------------

// Number

Element_p Number::evaluate(std::shared_ptr<Context> cx, int d)
{
   return std::make_shared<Number>(number);
}


// Boolean

Element_p Boolean::evaluate(std::shared_ptr<Context> cx, int d)
{
   return shared_from_this();
}


// List

Element_p List::evaluate(std::shared_ptr<Context> cx, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "List evaluate()\n";

   return shared_from_this();
}


// Call

Element_p Call::evaluate(std::shared_ptr<Context> cx, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Call evaluate()\n";

   Element_p  el = get(0);
   while (std::dynamic_pointer_cast<Call>(el) != nullptr)
   {
      // This is a nested call
      el = el->evaluate(cx, d + 1);
   }
   
   Callable_p ca = std::dynamic_pointer_cast<Callable>(el);
   if (ca != nullptr)
   {
      Bind_p   bi  = std::dynamic_pointer_cast<Bind>(ca);
      Lambda_p la  = std::dynamic_pointer_cast<Lambda>(ca);
      Symbol_p sy  = std::dynamic_pointer_cast<Symbol>(ca);
      Defn_p   df  = nullptr;
      Fn_p     fuu = nullptr;
      if (sy != nullptr)
      {
         Element_p fun = cx->search(sy->getText());
         if (fun == nullptr)
         {
            std::cout << "function " << sy->getText() << " not found\n";
            throw std::make_shared<RunError>();
         }

         if (debug) indent(d + 1);
         if (debug) std::cout << "name found\n";
         
         
         bi = std::dynamic_pointer_cast<Bind>(fun);
         if (bi != nullptr)
         {
            fuu = bi->getLambda()->getFn();
         }
         else
         {
            la = std::dynamic_pointer_cast<Lambda>(fun);
            if (la != nullptr)
            {
               bi = std::dynamic_pointer_cast<Bind>(la->capture(cx, nullptr, d + 1));
               fuu = la->getFn();
               la = nullptr;
            }
            else
            {
               df = std::dynamic_pointer_cast<Defn>(fun);
               if (df != nullptr)
               {
                  fuu = df->getFn();
               }
               else
               {
                  std::cout << "name found but is not a function\n";
                  throw std::make_shared<RunError>();
               }
            }
         }
      }
      else
      if (bi != nullptr)
      {
         fuu = bi->getLambda()->getFn();
      }
      else
      {
         if (la != nullptr)
         {
            fuu = la->getFn();
         }
      }

      // handle the parameters
      int fparsize = fuu->getParamsSize();
      int aparsize = size() - 1;
      
      if (debug) indent(d + 1);
      if (debug) std::cout << "fparsize " << fparsize << " aparsize " << fparsize << "\n";
      
      if (aparsize == fparsize)
      {
         Frame_p fr = std::make_shared<Frame>();
         
         for (int i=0; i<fparsize; i++)
         {
            Element_p apar = get(i + 1);
            std::string fparname = fuu->getParam(i);
            if (debug) indent(d + 1);
            if (debug) std::cout << "fparname " << fparname << "\n";

            Element_p aparres = apar->evaluate(cx, d + 1);

            if (debug) indent(d + 1);
            if (debug) std::cout << "==== aparam result===\n";
            if (debug) aparres->show(d + 2);
            if (debug) indent(d + 1);
            if (debug) std::cout << "=====\n";

            fr->add_binding(fparname, aparres);
         }
         
         if (bi != nullptr)
         {
            cx->push(bi->getFrame());
         }
         
         cx->push(fr);
         Element_p rs = fuu->evaluate(cx, d + 1);
         cx->pop();

         if (bi != nullptr)
         {
            cx->pop();
         }
         
         if (debug) indent(d + 1);
         if (debug) std::cout << "++++ fn result+++\n";
         if (debug) rs->show(d + 2);
         if (debug) indent(d + 1);
         if (debug) std::cout << "+++++++++\n";
         
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


// Let

Element_p Let::evaluate(std::shared_ptr<Context> cx, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Let evaluate()\n";

      
   Frame_p fr = std::make_shared<Frame>();
         
   for (const auto &pr: variables)
   {
      Element_p   apar     = pr.second;
      std::string fparname = pr.first;
      if (debug) indent(d + 1);
      if (debug) std::cout << "fparname " << fparname << "\n";

      Element_p aparres = apar->evaluate(cx, d + 1);

      if (debug) indent(d + 1);
      if (debug) std::cout << "==== aparam result===\n";
      if (debug) aparres->show(d + 2);
      if (debug) indent(d + 1);
      if (debug) std::cout << "=====\n";

      fr->add_binding(fparname, aparres);
   }
   
   cx->push(fr);
   Element_p rs = body->evaluate(cx, d + 1);
   cx->pop();

   if (debug) indent(d + 1);
   if (debug) std::cout << "++++ let result+++\n";
   if (debug) rs->show(d + 2);
   if (debug) indent(d + 1);
   if (debug) std::cout << "+++++++++\n";
   
   return rs;
}




// Println
 
Element_p Println::evaluate(std::shared_ptr<Context> cx, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Println evaluate\n";

   Element_p result;
   for (Element_p el: body->getElements())
   {
      result = el->evaluate(cx, d + 1);
      result->print();
      std::cout << " ";
   }
   std::cout << "\n";

   return result;
}

// Vector

Element_p Vector::evaluate(std::shared_ptr<Context> cx, int d)
{
   return std::make_shared<Number>(0);
}

// Body

Element_p Body::evaluate(std::shared_ptr<Context> cx, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Body evaluate\n";

   Element_p result;
   for (Element_p el: elements)
   {
      result = el->evaluate(cx, d + 1);
   }
   return result;
}

// Binary

Element_p Binary::evaluate(std::shared_ptr<Context> cx, int d)
{
   return std::make_shared<Number>(0);
}



// Mul

Element_p Mul::evaluate(std::shared_ptr<Context> cx, int d)
{
   number_t value = 1;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx, d + 1);
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

Element_p Div::evaluate(std::shared_ptr<Context> cx, int d)
{
   return std::make_shared<Number>(0);
}

// Plus

Element_p Plus::evaluate(std::shared_ptr<Context> cx, int d)
{
   number_t value = 0;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx, d + 1);
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

Element_p Min::evaluate(std::shared_ptr<Context> cx, int d)
{
   number_t value = 0;
   int i = 0;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx, d + 1);
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

Element_p Equal::evaluate(std::shared_ptr<Context> cx, int d)
{
   bool bo = true;
   Number_p last = nullptr;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx, d + 1);

      if (debug) indent(d + 1);
      if (debug) std::cout << "Equal el2\n";
      if (debug) el2->show(d + 2);

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

Element_p Less::evaluate(std::shared_ptr<Context> cx, int d)
{
   return std::make_shared<Number>(0);
}

// Greater

Element_p Greater::evaluate(std::shared_ptr<Context> cx, int d)
{
   return std::make_shared<Number>(0);
}

// NotEqual

Element_p NotEqual::evaluate(std::shared_ptr<Context> cx, int d)
{
   return std::make_shared<Number>(0);
}

// GreaterEq

Element_p GreaterEq::evaluate(std::shared_ptr<Context> cx, int d)
{
   return std::make_shared<Number>(0);
}

// LessEq

Element_p LessEq::evaluate(std::shared_ptr<Context> cx, int d)
{
   return std::make_shared<Number>(0);
}

// Defn

Element_p Defn::evaluate(std::shared_ptr<Context> cx, int d)
{
   cx->add_binding(name, shared_from_this());
   
   return shared_from_this();
}

// Lambda

Element_p Lambda::evaluate(std::shared_ptr<Context> cx, int d)
{   
   return capture(cx, nullptr, d + 1);
}

// Fn

Element_p Fn::evaluate(std::shared_ptr<Context> cx, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Fn evaluate\n";
   if (debug) cx->show(d + 1);

   return body->evaluate(cx, d + 1);
}

// Bind

void Bind::show(int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Bind\n";

   frame->show(d + 1);
   lambda->show(d + 1);
}

Element_p Bind::evaluate(std::shared_ptr<Context> cx, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Bind evaluate\n";

   return shared_from_this();
}

// If

Element_p If::evaluate(std::shared_ptr<Context> cx, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "If evaluate\n";

   Element_p el = condition->evaluate(cx, d + 1);
   Boolean_p bo = std::dynamic_pointer_cast<Boolean>(el);
   if (bo == nullptr)
   {
      std::cout << "no boolean expression\n";
      throw std::make_unique<RunError>();
   }
   
   if (bo->getValue())
   {
      if (debug) indent(d + 1);
      if (debug) std::cout << "If yes\n";

      return yes->evaluate(cx, d + 1);
   }
   else
   if (no != nullptr)
   {
      if (debug) indent(d + 1);
      if (debug) std::cout << "If no\n";

      return no->evaluate(cx, d + 1);
   }
   else
   {
      // should be nil
      return std::make_shared<Number>(0);
   }
}

// Symbol

Element_p Symbol::evaluate(std::shared_ptr<Context> cx, int d)
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


// Text

Element_p Text::evaluate(std::shared_ptr<Context> cx, int d)
{
   return shared_from_this();
}


// Main

Element_p Main::evaluate(std::shared_ptr<Context> cx, int d)
{
   Element_p result;
   for (Element_p el: elements)
   {
      result = el->evaluate(cx, d + 1);
   }
   return result;
}



// --------------- capture() ------------------

Element_p List::capture(Context_p cx, Frame_p fr, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "List capture\n";

   List_p lst = std::make_shared<List>();
   for (Element_p el: elements)
   {
      Element_p el2 = el->capture(cx, fr, d + 1);
      lst->add(el2);
      
   }
   return lst;
}

Element_p Call::capture(Context_p cx, Frame_p fr, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Call capture\n";

   Call_p cal = std::make_shared<Call>();
   for (Element_p el: elements)
   {
      Element_p el2 = el->capture(cx, fr, d + 1);
      cal->add(el2);
      
   }
   return cal;
}

Element_p Elements::capture(Context_p cx, Frame_p fr, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Elements capture " << info() << "\n";

   Elements_p els = make_copy();
   for (Element_p el: elements)
   {
      Element_p el2 = el->capture(cx, fr, d + 1);
      els->add(el2);
   }
   if (debug) els->show(d + 1);
   return els;
}

Element_p If::capture(Context_p cx, Frame_p fr, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "If capture\n";

   If_p fi = std::make_shared<If>();
   
   Element_p cond = condition->capture(cx, fr, d + 1);
   fi->setCondition(cond);
   Element_p ye   = yes->capture(cx, fr, d + 1);
   fi->setYes(ye);
   if (no != nullptr)
   {
      Element_p n = no->capture(cx, fr, d + 1);
      setNo(n);
   }
   return fi;
}

Element_p Println::capture(Context_p cx, Frame_p fr, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Writeln capture\n";

   Println_p pri = std::make_shared<Println>();
   
   if (body != nullptr)
   {
      Element_p bo = body->capture(cx, fr, d + 1);
      pri->body = std::dynamic_pointer_cast<Body>(body->capture(cx, fr, d + 1));
      if (pri->body == nullptr)
      {
         std::cout << "Println capture body nullptr\n";
         throw std::make_unique<RunError>();
      }
   }
   
   return pri;
}

Element_p Let::capture(Context_p cx, Frame_p fr, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Let capture\n";

   Let_p lt      = std::make_shared<Let>();
   lt->full      = full;
   
   Frame_p frr = std::make_shared<Frame>();
   for (const auto &pr: variables)
   {
      if (debug) indent(d + 1);
      if (debug) std::cout << "Let capture variable " << pr.first << "\n";

      frr->add_binding(pr.first, nullptr);

      Element_p val = pr.second->capture(cx, fr, d + 1);
      if (val == nullptr)
      {
         std::cout << "Let capture val nullptr\n";
         throw std::make_unique<RunError>();
      }
      lt->addVariable(pr.first, val);
   }

   cx->push(frr);
   lt->body = std::dynamic_pointer_cast<Body>(body->capture(cx, fr, d + 1));
   cx->pop();

   if (lt->body == nullptr)
   {
      std::cout << "Let capture body nullptr\n";
      throw std::make_unique<RunError>();
   }

   if (debug) lt->show(d + 1);

   return lt;
}

Element_p Fn::capture(Context_p cx, Frame_p fr, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Fn capture\n";

   Fn_p fn = std::make_shared<Fn>();
   fn->full = full;
   fn->params = params;
   
   Frame_p frr = std::make_shared<Frame>();
   for (std::string param: params)
   {
      if (debug) indent(d + 1);
      if (debug) std::cout << "Fn capture param " << param << "\n";

      frr->add_binding(param, nullptr);
   }

   cx->push(frr);
   fn->body = std::dynamic_pointer_cast<Body>(body->capture(cx, fr, d + 1));
   cx->pop();

   if (fn->body == nullptr)
   {
      std::cout << "Fn capture body nullptr\n";
      throw std::make_unique<RunError>();
   }

   if (debug) fn->show(d + 1);

   return fn;
}

Element_p Defn::capture(Context_p cx, Frame_p fr, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Dfn capture\n";

   Defn_p dfn = std::make_shared<Defn>();
   dfn->name = name;
   
   Element_p f = fn->capture(cx, fr, d + 1);
   dfn->fn = std::dynamic_pointer_cast<Fn>(f);
   
   return dfn;
}

Element_p Lambda::capture(Context_p cx, Frame_p fr, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Lambda capture\n";

   Lambda_p la = std::make_shared<Lambda>();
   Bind_p   bi = std::make_shared<Bind>();
   
   bi->setLambda(la);
   
   Frame_p fr2 = std::make_shared<Frame>();
   bi->setFrame(fr2);
   
   Element_p f = fn->capture(cx, fr2, d + 1);
   la->fn = std::dynamic_pointer_cast<Fn>(f);
   if (debug) bi->show(d + 1);
   return bi;
}

Element_p Symbol::capture(Context_p cx, Frame_p fr, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Symbol capture " << text << "\n";

   if (debug) cx->show(d + 1);
   
   bool exi = cx->exists(text);
   if (exi)
   {
      if (debug) indent(d + 1);
      if (debug) std::cout << "   Symbol capture found " << text << "\n";

      Element_p val = cx->search(text);
      if (val != nullptr)
      {
         if (debug) indent(d + 1);
         if (debug) std::cout << "   val not null\n";
         if (debug) val->show(d + 2);
         
         // add new binding
         fr->add_binding(text, val);

         if (debug) fr->show(d + 2);
      }
      else
      {
         if (debug) indent(d + 1);
         if (debug) std::cout << "   val null\n";
      }
   }
   else
   {
      if (debug) indent(d + 1);
      if (debug) std::cout << "Symbol capture not found " << text << "\n";
   }
   return shared_from_this();
}

Element_p Text::capture(Context_p cx, Frame_p fr, int d)
{
   if (debug) indent(d);
   if (debug) std::cout << "Text capture " << text << "\n";

   return shared_from_this();
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
   //std::cout << "Frame search\n";
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

bool Frame::exists(std::string nm)
{
   //show(0);
   auto it = bindings.find(nm);
   if (it != bindings.end())
   {
      return true;
   }
   else
   {
      return false;
   }
}

void Frame::show(int d)
{
   indent(d);
   std::cout << "Frame\n";
   
   for (auto it: bindings)
   {
      indent(d + 1);
      std::cout << it.first << "\n";
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
   //std::cout << "Context search\n";
   for (Frame_p fra: frames)
   {
      Element_p el = fra->search(nm);
      if (el != nullptr)
      {
         return el;
      }
   }
   return nullptr;
}

bool Context::exists(std::string nm)
{
   for (Frame_p fra: frames)
   {
      bool exi = fra->exists(nm);
      if (exi)
      {
         return true;
      }
   }
   return false;
}

void Context::show(int d)
{
   indent(d);
   std::cout << "Context #" << frames.size() << "\n";
   
   for (Frame_p fr: frames)
   {
      fr->show(d + 1);
   }
}

// Runner

Runner::Runner(Element_p rt) : root(rt)
{
}

Element_p Runner::run()
{
   if (debug) std::cout << "start runner\n";
   Context_p cx = std::make_shared<Context>();
   cx->push();
   Element_p rs = root->evaluate(cx, 0);
   cx->pop();
   return rs;
}
