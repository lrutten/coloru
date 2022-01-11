#include <iostream>

#include "easylogging++.h"

#include "parser.h"


/*
   getType()

   This function returns the basic type for every element.

 */

type_t Main::getType()
{
   return tp_pass;
}

type_t Text::getType()
{
   return tp_simple;
}

type_t Builtin::getType()
{
   return tp_pass;
}

type_t Symbol::getType()
{
   return tp_simple;
}

type_t Let::getType()
{
   return tp_pass;
}

type_t Ampersand::getType()
{
   return tp_simple;
}

type_t Println::getType()
{
   return tp_pass;
}

type_t If::getType()
{
   return tp_pass;
}

type_t Bind::getType()
{
   return tp_pass;
}

type_t Fn::getType()
{
   return tp_pass;
}

type_t Lambda::getType()
{
   return tp_pass;
}

type_t Defn::getType()
{
   return tp_pass;
}

type_t Binary::getType()
{
   return tp_simple;
}

type_t Body::getType()
{
   return tp_pass;
}

type_t Vector::getType()
{
   return tp_pass;
}

type_t Call::getType()
{
   return tp_simple;
}

type_t List::getType()
{
   return tp_simple;
}

type_t Nil::getType()
{
   return tp_simple;
}

type_t Boolean::getType()
{
   return tp_simple;
}

type_t Number::getType()
{
   return tp_simple;
}



/*
 resetTreetype()

 This method resets the treetype in all the elements to tp_undefined.
 The treetype reflects the state of the tree of which this element
 is the top.

 In some classes this method will recurse.
 */

void Main::resetTreetype()
{
   defines.clear();
   //CLOG(DEBUG, "recurse") << "defines clear size " << defines.size();
   setTreetype(tp_undefined);

   for (Element_p el: getElements())
   {
      el->resetTreetype();
   }
}

void Builtin::resetTreetype()
{
   setTreetype(tp_undefined);
}

void Symbol::resetTreetype()
{
   setTreetype(tp_undefined);
}

void Let::resetTreetype()
{
   setTreetype(tp_undefined);
   body->resetTreetype();
}

void Ampersand::resetTreetype()
{
   setTreetype(tp_undefined);
}

void Println::resetTreetype()
{
   setTreetype(tp_undefined);
   body->resetTreetype();
}

void If::resetTreetype()
{
   setTreetype(tp_undefined);
   condition->resetTreetype();
   yes->resetTreetype();
   no->resetTreetype();
}

void Bind::resetTreetype()
{
   setTreetype(tp_undefined);
}

void Fn::resetTreetype()
{
   setTreetype(tp_undefined);
   body->resetTreetype();
}

void Lambda::resetTreetype()
{
   setTreetype(tp_undefined);
   fn->resetTreetype();
}

/*
   The list of calls to this define will be cleared as well.
 */
void Defn::resetTreetype()
{
   calls.clear();
   setTreetype(tp_undefined);
   fn->resetTreetype();
}

void Binary::resetTreetype()
{
   setTreetype(tp_undefined);

   for (Element_p el: getElements())
   {
      el->resetTreetype();
   }
}

void Body::resetTreetype()
{
   setTreetype(tp_undefined);

   for (Element_p el: getElements())
   {
      el->resetTreetype();
   }
}

void Vector::resetTreetype()
{
   setTreetype(tp_undefined);

   for (Element_p el: getElements())
   {
      el->resetTreetype();
   }
}

void Call::resetTreetype()
{
   setTreetype(tp_undefined);

   for (Element_p el: getElements())
   {
      el->resetTreetype();
   }
}

void List::resetTreetype()
{
   setTreetype(tp_undefined);

   for (Element_p el: getElements())
   {
      el->resetTreetype();
   }
}

void Value::resetTreetype()
{
   setTreetype(tp_undefined);
}





/*
   determTreetype()

   This method determines the treetype for all the elements.
   This way it is possible to detect recursive functions
   and more specificially tail or non tail.

   Every element will have one of three types:
   * tp_simple
   * tp_recurse
   * tp_tailrecurse
 */
void Main::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   type_t tm = tp_simple;
   for (Element_p el: getElements())
   {
      el->determTreetype(main, defn);
      if (el->getTreetype() == tp_recurse ||
          el->getTreetype() == tp_tailrecurse)
      {
         setTreetype(el->getTreetype());
      }
   }
}

void Builtin::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   setTreetype(tp_simple);
}

void Symbol::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   setTreetype(tp_simple);
}

void Let::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   body->determTreetype(main, defn);
   setTreetype(body->getTreetype());
}

void Ampersand::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   setTreetype(tp_simple);
}

void Println::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   setTreetype(tp_simple);
   body->determTreetype(main, defn);
   setTreetype(body->getTreetype());
}

void If::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   condition->determTreetype(main, defn);
   yes->determTreetype(main, defn);
   no->determTreetype(main, defn);

   type_t tp = tp_simple;
   type_t tp2 = condition->getTreetype();
   if (tp2 == tp_recurse || tp2 == tp_tailrecurse)
   {
      tp = tp2;
   }

   tp2 = yes->getTreetype();
   if (tp2 == tp_recurse)
   {
      tp = tp2;
   }
   else
   if (tp2 == tp_tailrecurse && tp2 != tp_recurse)
   {
      tp = tp_tailrecurse;
   }

   tp2 = no->getTreetype();
   if (tp2 == tp_recurse)
   {
      tp = tp2;
   }
   else
   if (tp2 == tp_tailrecurse && tp2 != tp_recurse)
   {
      tp = tp_tailrecurse;
   }
   setTreetype(tp);
}

void Bind::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   setTreetype(tp_simple);
}

void Fn::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   body->determTreetype(main, defn);
   setTreetype(body->getTreetype());
}

void Lambda::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   setTreetype(tp_simple);
   fn->determTreetype(main, defn);
   setTreetype(fn->getTreetype());
}

void Defn::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   Defn_p ths = std::dynamic_pointer_cast<Defn>(shared_from_this());
   main->addDefn(ths);

   fn->determTreetype(main, ths);
   setTreetype(fn->getTreetype());
}

void Binary::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   for (Element_p el: getElements())
   {
      el->determTreetype(main, defn);
      if (el->getTreetype() == tp_recurse ||
          el->getTreetype() == tp_tailrecurse)
      {
         setTreetype(el->getTreetype());
      }
   }

   if (getTreetype() == tp_tailrecurse)
   {
      setTreetype(tp_recurse);
   }
}

void Body::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   setTreetype(tp_simple);
   for (Element_p el: getElements())
   {
      el->determTreetype(main, defn);
      if (el->getTreetype() == tp_recurse ||
          el->getTreetype() == tp_tailrecurse)
      {
         setTreetype(el->getTreetype());
      }
   }
}

void Vector::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   for (Element_p el: getElements())
   {
      el->determTreetype(main, defn);
      if (el->getTreetype() == tp_recurse ||
          el->getTreetype() == tp_tailrecurse)
      {
         setTreetype(el->getTreetype());
      }
   }

   if (getTreetype() == tp_tailrecurse)
   {
      setTreetype(tp_recurse);
   }
}

void Call::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   Call_p ths = std::dynamic_pointer_cast<Call>(shared_from_this());
   setTreetype(tp_simple);

   // all calls are added to the define via main
   main->addCall(ths);

   Element_p first = elements[0];
   Symbol_p sy = std::dynamic_pointer_cast<Symbol>(first);
   if (sy != nullptr && defn != nullptr && sy->getText() == defn->getName())
   {
      // the name in the call and the name in the define are equal
      // recursive call
      setTreetype(tp_tailrecurse);
   }
   else
   {
      // non-recursive call
      for (Element_p el: getElements())
      {
         el->determTreetype(main, defn);
         if (el->getTreetype() == tp_recurse ||
             el->getTreetype() == tp_tailrecurse)
         {
            setTreetype(el->getTreetype());
         }
      }

      if (getTreetype() == tp_tailrecurse)
      {
         setTreetype(tp_recurse);
      }
   }
}

void List::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   setTreetype(tp_simple);

   for (Element_p el: getElements())
   {
      el->determTreetype(main, defn);
      if (el->getTreetype() == tp_recurse ||
          el->getTreetype() == tp_tailrecurse)
      {
         setTreetype(el->getTreetype());
      }
   }

   if (getTreetype() == tp_tailrecurse)
   {
      setTreetype(tp_recurse);
   }
}

void Value::determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn)
{
   setTreetype(tp_simple);
}



/*
   transformTree()

   This method transforms a non tail recursive define into a tail recursive one.

   Each method returns a bool. A true indicates that the extra continuation parameter
   k must be added in the define.

   The method only transforms one non tail call into a tail recursive one.
   So fibonnaci will not be handled correctly.

   At the moment only non tail recursion caused by binary is resolved.

   The search algoritm is complex. Branches with tp_recurse are follewed until
   a
 */

bool Main::transformTree(int d)
{
   Element::transformTree(d);

   bool rs = false;
   for (Element_p el: getElements())
   {
      if (el->getTreetype() == tp_recurse)
      {
         rs = el->transformTree(d + 1);
         if (rs)
         {
            setCont(true);
         }
      }
   }

   return hasCont();
}

bool Fn::transformTree(int d)
{
   Element::transformTree(d);

   bool rs = false;
   if (body->getTreetype() == tp_recurse)
   {
      rs = body->transformTree(d + 1);
      if (rs)
      {
         setCont(true);
      }
   }
   return hasCont();
}

bool Defn::transformTree(int d)
{
   Element::transformTree(d);

   bool rs = false;
   if (fn->getTreetype() == tp_recurse)
   {
      rs = fn->transformTree(d + 1);
      if (rs)
      {
         setCont(true);
      }
   }

   return hasCont();
}

bool Body::transformTree(int d)
{
   Element::transformTree(d);

   int j = 0;
   Element_p el2 = nullptr;
   for (Element_p el: getElements())
   {
      if (el->getTreetype() == tp_recurse && el->getType() != tp_pass)
      {
         el2 = el;
         break;
      }
      bool rs = el->transformTree(d + 1);
      if (rs)
      {
         setCont(true);
      }
      j++;
   }
   if (el2 != nullptr)
   {
      CLOG(DEBUG, "recurse") << i(d + 1) << "found";
      setCont(true);
   }

   return hasCont();
}

/*
  if has tp_pass as basic type.
 */

bool If::transformTree(int d)
{
   Element::transformTree(d);

   if (yes->getTreetype() == tp_recurse && yes->getType() != tp_pass)
   {
      // this is a recursive branch which is not pass.
      // yes will be replaced by a tail recursive call which
      // includes the eisting yes branch
      CLOG(DEBUG, "recurse") << i(d + 1) << "found yes";

      // make the tail recursive call.
      // parameter yes is necessary to include it in the call
      Element_p call = yes->searchTail(yes, d + 1);
      yes = call;

      setCont(true);
   }
   else
   if (yes->getType() == tp_pass)
   {
      yes->transformTree(d + 1);
   }
   else
   {
      CLOG(DEBUG, "recurse") << i(d + 1) << "yes call k 1";

      // make extra call to k
      // the yes branch is added as only parameter
      Call_p  ca  = std::make_shared<Call>();
      ca->add(std::make_shared<Symbol>("k"));
      ca->add(yes);

      yes = ca;

      // the uppper define gets an extra parameter k
      setCont(true);
   }


   // the no branch gets the same treatment as the yes branch
   if (no->getTreetype() == tp_recurse && no->getType() != tp_pass)
   {
      CLOG(DEBUG, "recurse") << i(d + 1) << "found no";

      Element_p call = no->searchTail(no, d + 1);
      call->show(d + 1, "recurse");
      no->show(d + 1, "recurse");
      no = call;

      setCont(true);
   }
   else
   if (no->getType() == tp_pass)
   {
      no->transformTree(d + 1);
   }
   else
   {
      CLOG(DEBUG, "recurse") << i(d + 1) << "no call k 1";

      // make extra call to k
      Call_p  ca  = std::make_shared<Call>();
      ca->add(std::make_shared<Symbol>("k"));
      ca->add(yes);

      no = ca;

      setCont(true);
   }

   return hasCont();
}



/*
  searchTail()

  This method searches for the tail recurse in one of the sub branches.
 */
std::shared_ptr<Element> Binary::searchTail(std::shared_ptr<Element> el, int d)
{
   Element_p call = nullptr;
   int j = 0;
   for (Element_p ell: getElements())
   {
      if (ell->getTreetype() == tp_tailrecurse)
      {
         call = ell;
         break;
      }
      else
      {
         call = ell->searchTail(el, d + 1);
         if (call != nullptr)
         {
            return call;
         }
      }

      j++;
   }

   if (call != nullptr)
   {
      CLOG(DEBUG, "recurse") << i(d + 1) << "Binary tail recursive call found";

      // replace the call with symbol 'value'
      Symbol_p sy = std::make_shared<Symbol>("value");
      set(j, sy);

      // make extra lambda
      Lambda_p la = std::make_shared<Lambda>();
      Fn_p     fn = std::make_shared<Fn>();
      la->setFn(fn);
      ParamList_p pars = std::make_shared<ParamList>();
      Param_p     par  = std::make_shared<Param>("value");
      pars->addParam(par);
      fn->setParamList(pars);
      Body_p      bod  = std::make_shared<Body>();
      fn->setBody(bod);
      Call_p      ca2  = std::make_shared<Call>();
      bod->add(ca2);
      ca2->add(std::make_shared<Symbol>("k"));
      ca2->add(el);

      // add the lambda as extra parameter to the tail call
      (std::dynamic_pointer_cast<Call>(call))->add(la);

      return call;
   }
   else
   {
      return call;
   }
}


/*
   makeTail()

   This method transforms in all the defines one non-tail recursion call into
   a tail recursion call.
 */

bool Main::makeTail()
{
   Main_p ths = std::dynamic_pointer_cast<Main>(shared_from_this());

   resetTreetype();
   determTreetype(ths, nullptr);

   CLOG(DEBUG, "recurse") << "=====show 0 before transform===";
   show(0, "recurse");

   CLOG(DEBUG, "recurse") << "=========transform=============";
   transformTree(0);

   CLOG(DEBUG, "recurse") << "=========show 1 after transform";
   show(0, "recurse");

   CLOG(DEBUG, "recurse") << "=========reset determ Treetype=";
   resetTreetype();
   determTreetype(ths, nullptr);

   CLOG(DEBUG, "recurse") << "====show 2 after reset determ==";
   show(0, "recurse");

   CLOG(DEBUG, "recurse") << "========extra param============";

   // Iterate through all the tailrecurse defines
   // and add a extra parameter to its simple calls.
   //
   // This is the extra parameter:
   //      (fn [value] value)

   CLOG(DEBUG, "recurse") << i(0) << "defines";
   for (const auto &pr: defines)
   {
      CLOG(DEBUG, "recurse") << i(1) << "defn " << pr.first;
      if (pr.second->getTreetype() == tp_tailrecurse && pr.second->hasCont())
      {
         CLOG(DEBUG, "recurse") << i(2) << "tail recursion";

         Defn_p df = pr.second;

         // for each call: add extra actual parameter
         for (Call_p cl: df->getCalls())
         {
            CLOG(DEBUG, "recurse") << i(3) << "call " << cl->type_to_s();
            if (cl->getTreetype() == tp_simple)
            {
               CLOG(DEBUG, "recurse") << i(4) << "simple";

               // add extra lambda as actual parameter
               Lambda_p la = std::make_shared<Lambda>();
               Fn_p     fn = std::make_shared<Fn>();
               la->setFn(fn);
               ParamList_p pars = std::make_shared<ParamList>();
               Param_p     par  = std::make_shared<Param>("value");
               pars->addParam(par);
               fn->setParamList(pars);
               Body_p      bod  = std::make_shared<Body>();
               fn->setBody(bod);
               bod->add(std::make_shared<Symbol>("value"));

               cl->add(la);
            }
         }

         // add extra formal parameter k to defn
         if (df->hasCont())
         {
            df->getFn()->getParamList()->addParam(std::make_shared<Param>("k"));
         }
      }
   }

   // This is an extra iteration for setting treetype
   // after the parsing tree has been changed.

   resetTreetype();
   determTreetype(ths, nullptr);

   CLOG(DEBUG, "recurse") << "=========show 3=================";
   show(0, "recurse");
   CLOG(DEBUG, "recurse") << "===============================";

   // write the transformed Clojure code
   CLOG(DEBUG, "recurse") << "=========format================";
   if (showclj) format(0);


   // check for remaining  non tail recursion
   for (const auto &pr: defines)
   {
      CLOG(DEBUG, "recurse") << i(1) << "defn check nontail" << pr.first;
      if (pr.second->getTreetype() == tp_recurse)
      {
         CLOG(DEBUG, "recurse") << i(1) << "defn check nontail found" << pr.first;
         return true;
      }
   }

   return false;
}

