#include <iostream>
#include <sstream>
#include <ranges>

#include "easylogging++.h"
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


// Nil

Element_p Nil::evaluate(std::shared_ptr<Context> cx, int d)
{
   return shared_from_this();
}

// List

Element_p List::evaluate(std::shared_ptr<Context> cx, int d)
{
   CLOG(DEBUG, "runner") << i(d) << "List evaluate()";

   return shared_from_this();
}

// Param

// Parameter ssingle is not in use anymore.
//
bool Param::assignParameters(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, List_p apars, int d, bool ssingle)
{
   CLOG(DEBUG, "runner") << i(d) << "Param assignParameters() " << ssingle << " lo " << listonly;

   CLOG(DEBUG, "runner") << i(d + 1) << "rest " << getRest();
   CLOG(DEBUG, "runner") << i(d + 1) << "name " << name;
   apars->show(d + 1, "runner");

   std::string fparname = name;

   List_p list = apars;

   if (!rest)
   {
      // no rest
      if (list->size() > 0)
      {
         Element_p apar = list->get(0);
         list->pop_front();

         Element_p aparres = apar->evaluate(cx, d + 1);

         CLOG(DEBUG, "runner") << i(d + 1) << "==== aparam result===";

         //CLOG(DEBUG, "runner") << i(d + 1) << "show " << aparres;
         aparres->show(d + 2, "runner");
         CLOG(DEBUG, "runner") << i(d + 1) << "=====";

         fr->add_binding(fparname, aparres);
      }
      else
      {
         std::cout << "not enough parameters passed to function\n";
         throw std::make_shared<RunError>();
      }
   }
   else
   {
      // rest
      if (list->size() == 0)
      {
         CLOG(DEBUG, "runner") << i(d + 1) << "add binding nil";

         fr->add_binding(fparname, std::make_shared<Nil>());
      }
      else
      {
         CLOG(DEBUG, "runner") << i(d + 1) << "add binding list (rest)";

         List_p list2 = std::make_shared<List>();
         for (Element_p apar: list->getElements())
         {
            Element_p aparres = apar->evaluate(cx, d + 1);

            list2->add(aparres);
         }

         fr->add_binding(fparname, list2);
      }
   }
   return true;
}

// ParamList

// Parameter ssingle is not in use anymore.
//
bool ParamList::assignParameters(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, List_p apar, int d, bool ssingle)
{
   CLOG(DEBUG, "runner") << i(d) << "ParamList assignParameters() " << ssingle << " lo " << listonly;
   apar->show(d + 1, "runner");

   List_p list = apar;
   //if (ssingle)      // old way of testing
   if (isListonly())   // new way of testing
   {
      // This part is only executed on all ParamList's
      // on a lower level which are not preceded by an ampersand.

      // This part tests on list and nil.
      CLOG(DEBUG, "runner") << i(d + 1) << "single";

      if (apar->size() == 0)
      {
         std::cout << "parameter for list missing\n";
         throw std::make_shared<RunError>();
      }
      Element_p el = apar->get(0);

      Element_p elres = el->evaluate(cx, d + 1);

      Nil_p ni = std::dynamic_pointer_cast<Nil>(elres);
      list = std::dynamic_pointer_cast<List>(elres);
      if (list == nullptr)
      {
         // not a list
         if (ni != nullptr)
         {
            //it is nil
            CLOG(DEBUG, "runner") << i(d + 1) << "parameter is nil instead of list";

            // Make a list with 1 nil
            //   '(nil)
            list = std::make_shared<List>();
            list->add(std::make_shared<Nil>());
         }
         else
         {
            // not a list nor nil, error
            el->show(d + 1, "runner");
            std::cout << "parameter must be list\n";
            throw std::make_shared<RunError>();
         }
      }

      apar->pop_front();
      CLOG(DEBUG, "runner") << i(d + 1) << "apar size after pop_front" << apar->size();
      list->show(d + 1, "runner");
   }
   else
   {
      CLOG(DEBUG, "runner") << i(d + 1) << "no single";
   }

   CLOG(DEBUG, "runner") << i(d + 1) << "list size after no/yes single" << list->size();

   for (AParam_p par: params)
   {
      if (par->getRest())
      {
         par->assignParameters(cx, fr, list, d + 2); // implicit false
      }
      else
      {
         par->assignParameters(cx, fr, list, d + 2, true);
      }
   }

   // Show the complete context
   // d + 1
   // only for a short test
   //cx->show(0 , "capture");

   return true;
}



// Call

Element_p Call::evaluate(std::shared_ptr<Context> cx, int d)
{
   CLOG(DEBUG, "runner") << i(d) << "Call evaluate() " << type_to_s();

   //cx->breek(shared_from_this());
   
   Element_p  el = get(0);
   while (std::dynamic_pointer_cast<Call>(el) != nullptr)
   {
      // This is a nested call
      el = el->evaluate(cx, d + 1);
   }

   Callable_p ca = std::dynamic_pointer_cast<Callable>(el);
   if (ca != nullptr)
   {
      CLOG(DEBUG, "runner") << i(d + 1) << "callable found";

      Builtin_p bin = std::dynamic_pointer_cast<Builtin>(el);
      if (bin != nullptr)
      {
         CLOG(DEBUG, "runner") << i(d + 1) << "callable is builtin";

         // no break for the debugger

         // 16/ 4/2024 discarded
         // cx->push_scope(sc_defn);

         Element_p rs = bin->evaluate2(cx, shared_from_this(), d + 1);

         // 16/ 4/2024 discarded
         // cx->pop_scope();
         return rs;
      }
      else
      {
         Bind_p   bi  = std::dynamic_pointer_cast<Bind>(ca);
         Lambda_p la  = std::dynamic_pointer_cast<Lambda>(ca);
         Symbol_p sy  = std::dynamic_pointer_cast<Symbol>(ca);
         Defn_p   df  = nullptr;
         Fn_p     fuu = nullptr;
         if (sy != nullptr)
         {
            Element_p fun = cx->search(sy->getText(), d + 1, "runner", true);
            if (fun == nullptr)
            {
               std::cout << "function " << sy->getText() << " not found\n";
               throw std::make_shared<RunError>();
            }

            CLOG(DEBUG, "runner") << i(d + 1) << "function with name " << sy->getText() << " found";

            bi = std::dynamic_pointer_cast<Bind>(fun);
            if (bi != nullptr)
            {
               CLOG(DEBUG, "runner") << i(d + 1) << "bind1";
               fuu = bi->getLambda()->getFn();
            }
            else
            {
               la = std::dynamic_pointer_cast<Lambda>(fun);
               if (la != nullptr)
               {
                  // probably never reached
                  CLOG(DEBUG, "runner") << i(d + 1) << "lambda2";
                  // old style capture
                  //bi = std::dynamic_pointer_cast<Bind>(la->capture(cx, nullptr, d + 1, 0));
                  fuu = la->getFn();
                  la = nullptr;
               }
               else
               {
                  df = std::dynamic_pointer_cast<Defn>(fun);
                  if (df != nullptr)
                  {
                     CLOG(DEBUG, "runner") << i(d + 1) << "defn";
                     fuu = df->getFn();
                  }
                  else
                  {
                     fun->show(d + 1, "runner");
                     std::cout << "name found but is not a function " << sy->getText() << "\n";
                     throw std::make_shared<RunError>();
                  }
               }
            }
         }
         else
         if (bi != nullptr)
         {
            // immediate call of lambda
            CLOG(DEBUG, "runner") << i(d + 1) << "bind2";
            fuu = bi->getLambda()->getFn();
         }
         else
         {
            if (la != nullptr)
            {
               CLOG(DEBUG, "runner") << i(d + 1) << "lambda3";
               fuu = la->getFn();
            }
         }

         // handle the parameters
         int fparsize = fuu->getParamsSize();
         int aparsize = size() - 1;

         CLOG(DEBUG, "runner") << i(d + 1) << "fparsize " << fparsize << " aparsize " << fparsize;

          // assign all actual parameters to the formal parameters
         Frame_p fr = std::make_shared<Frame>("params");
         fr->setFrType(fr_defn);
         fuu->assignParameters(cx, fr, shared_from_this(), d + 1);

         /*
         if (bi != nullptr)
         {
            cx->push(bi->getFrame(), d + 1, "runner");
         }
         */

         CLOG(DEBUG, "runner") << i(d + 1) << "fuu " << type_to_s();

         // start push - evaluate - pop part
         // original code without tail recursion optimization
         //   cx->push(fr);
         //   Element_p rs = fuu->evaluate(cx, d + 1);
         //   cx->pop();

         Element_p rs;

         if (df == nullptr)
         {
            // this is not a symbol call
            CLOG(DEBUG, "runner") << i(d + 1) << "no symbol call";

            fr->setFrType(fr_lambda);

            if (bi != nullptr)
            {
               cx->push_scope(bi->getScope());
            }
            else
            {
               cx->push_scope(sc_lambda);
            }
            cx->push(fr, d + 1, "runner");

            // break the execution for the debugger
            cx->breek(shared_from_this());

            rs = fuu->evaluate(cx, d + 1);
            cx->pop();
            cx->pop_scope();
         }
         else
         {
            // symbol call to defn
            CLOG(DEBUG, "runner") << i(d + 1) << "defn symbol call";

            // this is the tail recursion optimization
            bool it = true;
            while (it)
            {
               CLOG(DEBUG, "runner") << i(d + 1) << "iterate";
               if (getTreetype() == tp_tailrecurse)
               {
                  CLOG(DEBUG, "runner") << i(d + 2) << "tail call";

                  // return the new frame as result
                  rs = fr;
                  it = false;
               }
               else
               {
                  CLOG(DEBUG, "runner") << i(d + 2) << "no tail call";

                  // simple call

                  cx->push_scope(sc_defn);
                  cx->push(fr, d + 1, "runner");
                  
                  // break the execution for the debugger
                  cx->breek(shared_from_this());

                  rs = fuu->evaluate(cx, d + 1);
                  Frame_p fr2 = std::dynamic_pointer_cast<Frame>(rs);
                  //cx->pop();
                  cx->pop_scope();
                  if (fr2 != nullptr)
                  {
                     // the evaluate returned a frame,
                     // the while can continue
                     CLOG(DEBUG, "runner") << i(d + 2) << "frame returned";
                     fr = fr2;
                  }
                  else
                  {
                     it = false;
                  }
               }
            }
         }

         // end push - evaluate - pop part

         if (bi != nullptr)
         {
            cx->pop();
         }

         CLOG(DEBUG, "runner") << i(d + 1) << "++++ fn result+++";
         rs->show(d + 2, "runner");
         CLOG(DEBUG, "runner") << i(d + 1) << "+++++++++";

         return rs;
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
   CLOG(DEBUG, "runner") << i(d) << "Let evaluate()";


   Frame_p fr = std::make_shared<Frame>("let");
   fr->setFrType(fr_let);

   for (const auto &pr: variables)
   {
      Element_p   apar     = pr.second;
      //std::string fparname = pr.first;
      AParam_p fpar = pr.first;
      CLOG(DEBUG, "runner") << i(d + 1) << "fparname ";

      Element_p aparres = apar->evaluate(cx, d + 1);

      CLOG(DEBUG, "runner") << i(d + 1) << "==== aparam result===";
      aparres->show(d + 2, "runner");
      CLOG(DEBUG, "runner") << i(d + 1) << "=====";

      //fr->add_binding(fparname, aparres);
      List_p aparlst = std::make_shared<List>();
      aparlst->add(aparres);

      // assignparameters expects a list containing the actual parameters.
      List_p aparlst_cpy = std::dynamic_pointer_cast<List>(aparlst->deep_copy());
      fpar->assignParameters(cx, fr, aparlst_cpy, d + 1);
   }

   cx->push(fr, d + 1, "runner");
   Element_p rs = body->evaluate(cx, d + 1);
   cx->pop();

   CLOG(DEBUG, "runner") << i(d + 1) << "++++ let result+++";
   rs->show(d + 2, "runner");
   CLOG(DEBUG, "runner") << i(d + 1) << "+++++++++";

   return rs;
}




// Println

Element_p Println::evaluate(std::shared_ptr<Context> cx, int d)
{
   CLOG(DEBUG, "runner") << i(d) << "Println evaluate";

   Element_p result;
   for (Element_p el: body->getElements())
   {
      result = el->evaluate(cx, d + 1);
      //result->show(d + 2, "runner");
      result->print();
      std::cout << " ";
   }
   std::cout << "\n";

   //return result;
   return std::make_shared<Nil>();
}

// Print

Element_p Print::evaluate(std::shared_ptr<Context> cx, int d)
{
   CLOG(DEBUG, "runner") << i(d) << "Print evaluate";

   Element_p result;
   for (Element_p el: body->getElements())
   {
      result = el->evaluate(cx, d + 1);
      //result->show(d + 2, "runner");
      result->print();
      std::cout << " ";
   }
   //std::cout << "\n";

   //return result;
   return std::make_shared<Nil>();
}

// Vector

Element_p Vector::evaluate(std::shared_ptr<Context> cx, int d)
{
   //return std::make_shared<Number>(0);
   std::cout << "don't use vectors as a value type.\n";
   std::cout << "vectors as a value type are not yet implemented.\n";
   throw std::make_shared<RunError>();
}

// Body

Element_p Body::evaluate(std::shared_ptr<Context> cx, int d)
{
   CLOG(DEBUG, "runner") << i(d) << "Body evaluate";

   Element_p result;
   for (Element_p el: elements)
   {
      CLOG(DEBUG, "runner") << i(d + 1) << "Body element";
      el->show(d + 2, "runner");
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
         std::cout << "run error: not a number in * expression\n";
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
         std::cout << "run error: not a number in + expression\n";
         throw std::make_unique<RunError>();
      }
      //std::cout << "Plus " << nu->getNumber() << "\n";
      value += nu->getNumber();
   }
   //std::cout << "Plus sum " << value << "\n";
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
         std::cout << "run error not a number in - expression\n";
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

      CLOG(DEBUG, "runner") << i(d + 1) << "Equal el2";
      el2->show(d + 2, "runner");

      Number_p nu = std::dynamic_pointer_cast<Number>(el2);
      if (nu == nullptr)
      {
         std::cout << "only numbers can be compared with =\n";
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
   bool bo = true;
   Number_p last = nullptr;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx, d + 1);

      CLOG(DEBUG, "runner") << i(d + 1) << "Less el2";
      el2->show(d + 2, "runner");

      Number_p nu = std::dynamic_pointer_cast<Number>(el2);
      if (nu == nullptr)
      {
         std::cout << "only numbers can be compared with <\n";
         throw std::make_unique<RunError>();
      }
      if (last != nullptr && last->getNumber() >= nu->getNumber())
      {
         return std::make_shared<Boolean>(false);
      }
      last = nu;
   }
   return std::make_shared<Boolean>(true);
}

// Greater

Element_p Greater::evaluate(std::shared_ptr<Context> cx, int d)
{
   bool bo = true;
   Number_p last = nullptr;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx, d + 1);

      CLOG(DEBUG, "runner") << i(d + 1) << "Greater el2";
      el2->show(d + 2, "runner");

      Number_p nu = std::dynamic_pointer_cast<Number>(el2);
      if (nu == nullptr)
      {
         std::cout << "only numbers can be compared with >\n";
         throw std::make_unique<RunError>();
      }
      if (last != nullptr && last->getNumber() <= nu->getNumber())
      {
         return std::make_shared<Boolean>(false);
      }
      last = nu;
   }
   return std::make_shared<Boolean>(true);
}

// NotEqual

Element_p NotEqual::evaluate(std::shared_ptr<Context> cx, int d)
{
   bool bo = true;
   Number_p last = nullptr;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx, d + 1);

      CLOG(DEBUG, "runner") << i(d + 1) << "NotEqual el2";
      el2->show(d + 2, "runner");

      Number_p nu = std::dynamic_pointer_cast<Number>(el2);
      if (nu == nullptr)
      {
         std::cout << "only numbers can be compared with !=\n";
         throw std::make_unique<RunError>();
      }
      if (last != nullptr && last->getNumber() == nu->getNumber())
      {
         return std::make_shared<Boolean>(false);
      }
      last = nu;
   }
   return std::make_shared<Boolean>(true);
}

// GreaterEq

Element_p GreaterEq::evaluate(std::shared_ptr<Context> cx, int d)
{
   bool bo = true;
   Number_p last = nullptr;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx, d + 1);

      CLOG(DEBUG, "runner") << i(d + 1) << "Less el2";
      el2->show(d + 2, "runner");

      Number_p nu = std::dynamic_pointer_cast<Number>(el2);
      if (nu == nullptr)
      {
         std::cout << "only numbers can be compared with >=\n";
         throw std::make_unique<RunError>();
      }
      if (last != nullptr && last->getNumber() < nu->getNumber())
      {
         return std::make_shared<Boolean>(false);
      }
      last = nu;
   }
   return std::make_shared<Boolean>(true);
}

// LessEq

Element_p LessEq::evaluate(std::shared_ptr<Context> cx, int d)
{
   bool bo = true;
   Number_p last = nullptr;
   for (Element_p el: elements)
   {
      Element_p el2 = el->evaluate(cx, d + 1);

      CLOG(DEBUG, "runner") << i(d + 1) << "LessEq el2";
      el2->show(d + 2, "runner");

      Number_p nu = std::dynamic_pointer_cast<Number>(el2);
      if (nu == nullptr)
      {
         std::cout << "only numbers can be compared with <=\n";
         throw std::make_unique<RunError>();
      }
      if (last != nullptr && last->getNumber() > nu->getNumber())
      {
         return std::make_shared<Boolean>(false);
      }
      last = nu;
   }
   return std::make_shared<Boolean>(true);
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
   CLOG(DEBUG, "capture") << i(d) << "Lambda evaluate, lambda3";
   //Element_p res = capture(cx, nullptr, d + 1, 0);

   Bind_p   bi = std::make_shared<Bind>();
   bi->setLambda(std::dynamic_pointer_cast<Lambda>(shared_from_this()));
   bi->setScope(cx->current_scope()->copy());

   // Lambda::capture returns a Binding
   return bi;
}

// Fn

bool Fn::assignParameters(std::shared_ptr<Context> cx, std::shared_ptr<Frame> fr, Element_p callel, int d)
{
   Call_p call = std::dynamic_pointer_cast<Call>(callel);

   // copy the call parameters to a list
   List_p list = std::make_shared<List>();
   for (int i = 1; i<call->getElements().size(); i++)
   {
      Element_p el = call->get(i);
      list->add(el);
   }

   paramlist->assignParameters(cx, fr, list, d); // ssingle implicit false
   return true;
}

Element_p Fn::evaluate(std::shared_ptr<Context> cx, int d)
{
   CLOG(DEBUG, "runner") << i(d) << "Fn evaluate";
   cx->show(d + 1, "runner");

   return body->evaluate(cx, d + 1);
}

// Bind

void Bind::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Bind";

   if (scope != nullptr)
   {
       scope->show(d + 1, chan.c_str());
   }
   //lambda->show(d + 1, chan.c_str());
}

Element_p Bind::evaluate(std::shared_ptr<Context> cx, int d)
{
   CLOG(DEBUG, "runner") << i(d) << "Bind evaluate";

   return shared_from_this();
}

// If

Element_p If::evaluate(std::shared_ptr<Context> cx, int d)
{
   CLOG(DEBUG, "runner") << i(d) << "If evaluate";

   Element_p el = condition->evaluate(cx, d + 1);
   Boolean_p bo = std::dynamic_pointer_cast<Boolean>(el);
   if (bo == nullptr)
   {
      std::cout << "no boolean expression\n";
      throw std::make_unique<RunError>();
   }

   if (bo->getValue())
   {
      CLOG(DEBUG, "runner") << i(d + 1) << "If yes";

      return yes->evaluate(cx, d + 1);
   }
   else
   if (no != nullptr)
   {
      CLOG(DEBUG, "runner") << i(d + 1) << "If no";

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
   Element_p val = cx->search(text, d, "runner");
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


// Builtin

Element_p Builtin::evaluate(std::shared_ptr<Context> cx, int d)
{
   return shared_from_this();
}

Element_p Builtin::evaluate2(std::shared_ptr<Context> cx, std::shared_ptr<Element> call, int d)
{
   CLOG(DEBUG, "runner") << i(d) << "Builtin evaluate2";

   call->show(d + 1, "runner");

   Call_p ca = std::dynamic_pointer_cast<Call>(call);
   if (ca != nullptr)
   {
      int npar = 1; // number of parameters 1

      Builtin_p bi = std::dynamic_pointer_cast<Builtin>(ca->get(0));
      if (bi != nullptr)
      {
         std::string bitext = bi->getText();

         CLOG(DEBUG, "runner") << i(d + 1) << "builtin " << bitext;

         // keep only the parameters
         List_p ca2 = ca->copy();
         ca2->pop_front();

         ca->show(d + 1, "runner");

         if (bitext == "nil?")
         {
            npar = 1;
         }
         else
         if (bitext == "empty?")
         {
            npar = 1;
         }
         else
         if (bitext == "context!")
         {
            npar = 0;
         }
         else
         if (bitext == "cons")
         {
            npar = 2;
         }

         if (ca2->size() == npar)
         {
            CLOG(DEBUG, "runner") << i(d + 1) << "# params correct";
            ca->show(d + 2, "runner");

            List_p list2 = std::make_shared<List>();
            for (Element_p apar: ca2->getElements())
            {
               CLOG(DEBUG, "runner") << i(d + 2) << "par";

               Element_p aparres = apar->evaluate(cx, d + 1);
               list2->add(aparres);

               aparres->show(d + 3, "runner");
            }

            if (bitext == "nil?")
            {
               CLOG(DEBUG, "runner") << i(d + 1) << "nil? executes";

               Element_p el0 = list2->get(0);
               Nil_p ni = std::dynamic_pointer_cast<Nil>(el0);
               if (ni != nullptr)
               {
                  return std::make_shared<Boolean>(true);
               }
               else
               {
                  return std::make_shared<Boolean>(false);
               }
            }
            else
            if (bitext == "empty?")
            {
               CLOG(DEBUG, "runner") << i(d + 1) << "empty? executes";

               Element_p el0 = list2->get(0);

               el0->show(d + 2, "runner");

               // Nil or List is returning true
               List_p li = std::dynamic_pointer_cast<List>(el0);
               Nil_p ni = std::dynamic_pointer_cast<Nil>(el0);
               if (ni != nullptr)
               {
                  CLOG(DEBUG, "runner") << i(d + 2) << "empty? nil true";
                  return std::make_shared<Boolean>(true);
               }
               else
               if (li != nullptr)
               {
                  if (li->size() == 0)
                  {
                     CLOG(DEBUG, "runner") << i(d + 2) << "empty? list true";
                     return std::make_shared<Boolean>(true);
                  }
                  else
                  {
                     CLOG(DEBUG, "runner") << i(d + 2) << "empty? false";
                     return std::make_shared<Boolean>(false);
                  }
               }
               else
               {
                  std::cout << "empty? builtin expects a list\n";
                  throw std::make_shared<RunError>();
               }
            }
            else
            if (bitext == "context!")
            {
               CLOG(DEBUG, "runner") << i(d + 1) << "context! executes";

               std::stringstream ss;
               ss << "<#" << cx->size();
               // for (Frame_p fr: std::ranges::views::reverse(cx->getFrames()))
               for (Frame_p fr: cx->getFrames())
               {
                  ss << "|" << fr->getNr() << "-";
                  for (auto it: fr->getBindings())
                  {
                     std::string nm = it.first;
                     Element_p  val = it.second;
                     ss << nm;
                     Number_p nu = std::dynamic_pointer_cast<Number>(val);
                     if (nu != nullptr)
                     {
                        ss << "=(" << nu->type_to_s() << ")" << nu->getNumber();
                        if (nu->getFrame() != nullptr)
                        {
                           //ss << "%" << nu->getFrame()->getNr() << "$" << nu->getFrame()->getInfo();
                           ss << "%" << nu->getFrame()->getNr();
                        }
                     }
                     else
                     if (std::dynamic_pointer_cast<Bind>(val) != nullptr)
                     {
                        Bind_p bi = std::dynamic_pointer_cast<Bind>(val);
                        ss << "bind%" << bi->getFrame()->getNr();
                     }
                     else
                     {
                        if (val->info() != "Defn")
                        {
                           ss << ":" << val->info();
                        }
                     }
                     ss << " ";
                  }
                  ss << "\n";
               }
               ss << ">";
               return std::make_shared<Text>(ss.str());
            }
            else
            if (bitext == "cons")
            {
               CLOG(DEBUG, "runner") << i(d + 1) << "cons executes";

               Element_p el0 = list2->get(0);
               Element_p el1 = list2->get(1);

               el0->show(d + 2, "runner");
               el1->show(d + 2, "runner");

               List_p   li = std::dynamic_pointer_cast<List>(el1);
               Nil_p    ni = std::dynamic_pointer_cast<Nil>(el1);
               Number_p nu = std::dynamic_pointer_cast<Number>(el1);
               if (li != nullptr)
               {
                  CLOG(DEBUG, "runner") << i(d + 1) << "cons copy, add_front";
                  List_p li2 = li->copy();
                  li2->add_front(el0);
                  return li2;
               }
               else
               if (ni != nullptr)
               {
                  CLOG(DEBUG, "runner") << i(d + 1) << "cons no copy, add_front";
                  List_p li2 = std::make_shared<List>();
                  li2->add_front(el0);
                  return li2;
               }
               else
               if (nu != nullptr)
               {
                  std::cout << "cons builtin expects a list, not a number\n";
                  throw std::make_shared<RunError>();
               }
               else
               {
                  std::cout << "cons builtin expects a list\n";
                  throw std::make_shared<RunError>();
               }
            }
         }
         else
         {
            std::cout << "wrong number of parameters for builtin " << bitext << "\n";
            throw std::make_shared<RunError>();
         }
      }
      else
      {
         std::cout << "internal error, Builtin expected\n";
         throw std::make_shared<RunError>();
      }
   }
   else
   {
      std::cout << "internal error, Call expected\n";
      throw std::make_shared<RunError>();
   }
   return nullptr;
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


// Frame

int Frame::counter = 0;

Frame::Frame() : frtype(fr_undefined), info(""), nr(counter++)
{
}

Frame::Frame(std::string inf) : frtype(fr_undefined), info(inf), nr(counter++)
{
}

Frame::~Frame()
{
}

void Frame::add_binding(std::string nm, Element_p el)
{
   //CLOG(DEBUG, "capture") << "add_binding()";
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
   //show(0, "runner");
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

std::string Frame::frametp_as_s()
{
   switch (frtype)
   {
   case fr_undefined:
      return "u";
   case fr_main:
      return "m";
   case fr_defn:
      return "d";
   case fr_lambda:
      return "a";
   case fr_capture:
      return "c";
   case fr_let:
      return "l";
   }
   return "u";
}

void Frame::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "|Frame " << nr << " " << frametp_as_s();

   for (auto it: bindings)
   {
      CLOG(DEBUG, chan.c_str()) << i(d + 1) << it.first;
      if (it.second != nullptr)
      {
         if (std::dynamic_pointer_cast<Number>(it.second) != nullptr)
         {
            it.second->show(d + 2, chan.c_str());
         }
         else
         if (std::dynamic_pointer_cast<Bind>(it.second) != nullptr)
         {
            it.second->show(d + 2, chan.c_str());
         }
         else
         {
            CLOG(DEBUG, chan.c_str()) << i(d + 2) << "type " << it.second->info(); 
         }
      }
      else
      {
         CLOG(DEBUG, chan.c_str()) << i(d) << "no value"; 
      }
   }
}

// Sink

Sink::Sink(coro_t::push_type &snk) : sink(snk)
{
}


// Scope
int Scope::counter = 0;


Scope::Scope(scope_t tp) : nr(counter++)
{
    sctype = tp;
}

Scope::~Scope()
{
}

void Scope::push(frame_t frtp)
{
    Frame_p fr = std::make_shared<Frame>("first");
    fr->setFrType(frtp);
    frames.push_front(fr);
}

void Scope::push(Frame_p fr, int d, const std::string &chan)
{
    CLOG(DEBUG, chan.c_str()) << i(d + 1) << "Scope push fr " << fr->getNr();
    //fr->setNr(frames.size());
    frames.push_front(fr);
}

void Scope::pop()
{
    frames.pop_front();
}

void Scope::add_binding(std::string nm, Element_p el)
{
    frames.front()->add_binding(nm, el);
}

int scounter1 = 0;

Element_p Scope::search(std::string nm, int d, const std::string &chan, bool shortsrch)
{
    shortsrch = true;
    bool debug = true;

    //show(5, "capture");

    CLOG(DEBUG, chan.c_str()) << i(d) << "Scope search " << nm << " short " << shortsrch << " #" << frames.size();
    // search in all the recent frames
    // until the first defn or capture frame is encountered
    for (Frame_p fra: frames)
    {
        //CLOG(DEBUG, chan.c_str()) <<i(d + 1) << "fr " << fra->getNr();
        fra->show(d + 2, chan.c_str());
        Element_p el = fra->search(nm);
        if (el != nullptr)
        {
            //el->show(1, "capture");
            if (el->getFrame() == nullptr)
            {
                el->setFrame(fra);
            }
            return el;
        }

        /*
        if (fra->getFrType() == fr_defn || fra->getFrType() == fr_capture)
        {
            if (shortsrch)
            {
                break;
            }
        }
         */
    }

    // search in the main frame
    for (Frame_p fra: frames)
    {
        if (fra->getFrType() == fr_main)
        {
            Element_p el = fra->search(nm);
            if (el != nullptr)
            {
                //el->show(1, "runner");
                return el;
            }
        }
    }
    CLOG(DEBUG, chan.c_str()) << i(d) << "not found " << nm;
    return nullptr;
}

bool Scope::exists(std::string nm)
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

void Scope::show(int d, const std::string &chan)
{
    CLOG(DEBUG, chan.c_str()) << i(d) << "|Scope " << nr << " #" << frames.size();

    //for (Frame_p fr: frames)
    for (Frame_p fr: std::ranges::views::reverse(frames))
    {
        fr->show(d + 1, chan);
    }
}

Scope_p Scope::copy()
{
    Scope_p sc = std::make_shared<Scope>(sc_capture);

    //CLOG(DEBUG, "runner") << "Scope copy a this " << frames.size();
    //CLOG(DEBUG, "runner") << "Scope copy a sc   " << sc->frames.size();
    sc->frames = frames;
    //CLOG(DEBUG, "runner") << "Scope copy b this " << frames.size();
    //CLOG(DEBUG, "runner") << "Scope copy b sc   " << sc->frames.size();

    return sc;
}

// Context

Context::Context() : sink(nullptr)
{
   Scope_p sc = std::make_shared<Scope>(sc_main);
   scopes.push_front(sc);
}

Context::~Context()
{
}

void Context::push(frame_t frtp)
{
   //Frame_p fr = std::make_shared<Frame>("first");
   //fr->setFrType(frtp);
   scopes.front()->push(frtp);
}

void Context::push(Frame_p fr, int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d + 1) << "Context push fr " << fr->getNr();
   //fr->setNr(frames.size());
   scopes.front()->push(fr, d, chan);
}

void Context::pop()
{
   scopes.front()->pop();
}

void Context::push_scope(scope_t tp)
{
   CLOG(DEBUG, "runner") << "Context push_scope primo";
   Scope_p sc = std::make_shared<Scope>(tp);
   scopes.push_front(sc);
}

void Context::push_scope(Scope_p sc)
{
    CLOG(DEBUG, "runner") << "Context push_scope secundo";
    scopes.push_front(sc);
}

void Context::pop_scope()
{
   CLOG(DEBUG, "runner") << "Context pop_scope " << scopes.size();
   scopes.pop_front();
}
Scope_p Context::current_scope()
{
    return scopes.front();
}

void Context::add_binding(std::string nm, Element_p el)
{
    scopes.front()->add_binding(nm, el);
}

int scounter = 0;

Element_p Context::search(std::string nm, int d, const std::string &chan, bool shortsrch)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Context search " << nm << " short " << shortsrch;
   Element_p el = scopes.front()->search(nm, d, chan, shortsrch);
   if (el != nullptr)
   {
      return el;
   }

   // search in the main frame
   Element_p el2 = scopes.back()->search(nm, d, chan, shortsrch);
   if (el2 != nullptr)
   {
      return el2;
   }
   CLOG(DEBUG, chan.c_str()) << i(d) << "not found " << nm;
   return nullptr;
}

bool Context::exists(std::string nm)
{
   for (Scope_p sco: scopes)
   {
      bool exi = sco->exists(nm);
      if (exi)
      {
         return true;
      }
   }
   return false;
}

void Context::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "|Context #" << scopes.size();

   for (Scope_p sc: std::ranges::views::reverse(scopes))
   {
      sc->show(d + 1, chan);
   }
}

void Context::breek(Element_p cur)
{
   // is debugger active?
   if (sink != nullptr)
   {
      setCurrent(cur);
      sink->getSink()(shared_from_this());
      //sink->getSink()(true);
   }
}

// Runner

Runner::Runner(Element_p rt) : root(rt)
{
}

Element_p Runner::run()
{
   CLOG(DEBUG, "runner") << "start runner";
   Context_p cx = std::make_shared<Context>();
   cx->push(fr_main);
   Element_p rs = root->evaluate(cx, 0);
   cx->pop();
   return rs;
}

Element_p Runner::debugger()
{
   CLOG(DEBUG, "runner") << "start debugger";
   
   coro_t::pull_type runner(
      [this](coro_t::push_type &sink)
      {
         Sink_p si = std::make_shared<Sink>(sink);
         
         Context_p cx = std::make_shared<Context>();
         cx->setSink(si);
         
         cx->push(fr_main);
         Element_p rs = root->evaluate(cx, 0);
         cx->pop();

         cx->setRunning(false);
         sink(cx);
      });

   bool running = true;
   while (running)
   {
      Context_p cx = runner.get();
      if (cx != nullptr)
      {
         if (!cx->getRunning())
         {
            std::cout << "runner breek stop\n";
            running = false;
            break;
         }

         std::cout << "current " << cx->getCurrent()->info() << "\n";
         std::cout << ">";

         //cx->getCurrent()->show(0, "main");
         //cx->show(0, "main");

         std::string line;
         std::cin >> line;

         bool debugmode = true;
         while (debugmode)
         {
            if (line == "q")
            {
               // quit
               debugmode = false;
               running = false;
            }
            else
            if (line == "c")
            {
               // do next step
               debugmode = false;
            }
            else
            if (line == "r")
            {
               // do run
               bool ru = true;
               while (ru)
               {
                  runner();

                  Context_p cx = runner.get();
                  if (cx != nullptr)
                  {
                     //std::cout << "current " << cx->getCurrent()->info() << "\n";
                     
                     if (!cx->getRunning())
                     {
                        std::cout << "runner breek stop\n";
                        ru = false;
                        exit(0);
                     }
                  }
               }
            }
            else
            if (line == "o")
            {
               // show context
               cx->show(0, "debugger");
            }
            else
            if (line == "u")
            {
               // show current element
               cx->getCurrent()->show(0, "debugger");
            }
            else
            if (line == "h")
            {
               // show help
               std::cout << "   q    exit\n";
               std::cout << "   c    step\n";
               std::cout << "   r    run\n";
               std::cout << "   o    show context\n";
               std::cout << "   u    show element\n";
               std::cout << "   h    help\n";
            }
            else
            {
               Element_p el = cx->search(line, 0, "debugger");
               if (el != nullptr)
               {
                  el->show(0, "debugger");
               }
               else
               {
                  std::cout << "name not " << line << " found\n";
               }
            }

            if (debugmode)
            {
               std::cout << ">";
               std::cin >> line;
            }
         }
      }
      else
      {
         std::cout << "runner breek cx nullptr\n";
         running = false;
      }
      
      if (running)
      {
         // continue the coroutine
         runner();
      }
   }
   
   return nullptr;
}
