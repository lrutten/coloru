#include <iostream>
#include <getopt.h>

#include "easylogging++.h"

#include "parser.h"
#include "runner.h"


void indent(int d)
{
   while (d-- > 0)
   {
      std::cout << " ";
   }
}

std::string i(int d)
{
   std::string ids = "";
   while (d-- > 0)
   {
      ids += " ";
   }
   return ids;
}

Element::Element() : treetype(tp_undefined), hascont(false), frame(nullptr)
{
}

Element::~Element()
{
}

// Number

Number::Number(number_t nu)
{
   number = nu;
}


Number::~Number()
{
   //std::cout << "~Number " << number << "\n";
}

std::string Element::type_to_s()
{
   std::string k = "-";
   if (hascont)
   {
      k = "k";
   }

   switch (treetype)
   {
   case tp_undefined:   return "u" + k;
   case tp_simple:      return "s" + k;
   case tp_pass:        return "p" + k;
   case tp_recurse:     return "r" + k;
   case tp_tailrecurse: return "t" + k;
   default:             return "x" + k;
   }
}

void Number::show(int d, const std::string &chan)
{
   //CLOG(DEBUG, chan.c_str()) << "start coloru bis";
   CLOG(DEBUG, chan.c_str())  << i(d) << "Number:" << type_to_s() << " " << number;
   if (getFrame() != nullptr)
   {
      CLOG(DEBUG, chan.c_str())  << i(d + 1) << "%" << getFrame()->getNr() << "-" << getFrame()->getInfo();
   }
}

void Number::print()
{
   std::cout << number;
   /*
   if (getFrame() != nullptr)
   {
      std::cout << "%" << getFrame()->getNr() << "-" << getFrame()->getInfo();
   }
    */
}

void Number::format(int d)
{
   std::cout << number;
}

// Boolean

Boolean::Boolean(bool val) : value(val)
{
}

Boolean::~Boolean()
{
   //std::cout << "~Boolean " << value << "\n";
}

void Boolean::show(int d, const std::string &chan)
{
   if (value != 0)
   {
      CLOG(DEBUG, chan.c_str()) << i(d) << "Boolean:" << type_to_s() << " true" ;
   }
   else
   {
      CLOG(DEBUG, chan.c_str()) << i(d) << "Boolean:" << type_to_s() << " false";
   }
}

void Boolean::format(int d)
{
   if (value != 0)
   {
      std::cout << "true";
   }
   else
   {
      std::cout << "false";
   }
}


// Nil

Nil::Nil()
{
}

Nil::~Nil()
{
}

void Nil::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Nil:" << type_to_s();
}

void Nil::format(int d)
{
   std::cout << "nil";
}


// List

List::List()
{
}

List::~List()
{
   //std::cout << "~List\n";
}

void List::add(Element_p el)
{
   elements.push_back(el);
}

void List::add_front(Element_p el)
{
   elements.push_front(el);
}

std::size_t List::size()
{
   return elements.size();
}

Element_p List::get(int i)
{
   return elements[i];
}

void List::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "List:" << type_to_s() << " #" << size();

   for (Element_p el: elements)
   {
      el->show(d + 1, chan);
   }
}

void List::format(int d)
{
   std::cout << "'(";
   for (Element_p el: elements)
   {
      el->format(d + 1);
      std::cout << " ";
   }
   std::cout << ")";
}

void List::print()
{
   std::cout << '(';
   for (Element_p el: elements)
   {
      el->print();
      std::cout << " ";
   }
   std::cout << ')';
}

List_p List::copy()
{
   List_p li = std::make_shared<List>();
   for (Element_p el: elements)
   {
      li->add(el);
   }

   return li;
}

Element_p List::deep_copy()
{
   List_p li = std::make_shared<List>();
   for (Element_p el: elements)
   {
      li->add(el->deep_copy());
   }

   return li;
}

// Call

Call::Call()
{
}

Call::~Call()
{
   //std::cout << "~List\n";
}

void Call::add(Element_p el)
{
   elements.push_back(el);
}

std::size_t Call::size()
{
   return elements.size();
}

Element_p Call::get(int i)
{
   return elements[i];
}

List_p Call::copy()
{
   List_p li = std::make_shared<List>();
   for (Element_p ell: getElements())
   {
      li->add(ell);
   }

   return li;
}


void Call::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Call:" << type_to_s();

   for (Element_p el: elements)
   {
      el->show(d + 1, chan);
   }
}

void Call::format(int d)
{
   indent(d);
   std::cout << "(";
   int i = 0;
   for (Element_p el: elements)
   {
      if (i != 0)
      {
         indent(d + 1);
      }
      el->format(d + 1);
      std::cout << "\n";
      i++;
   }
   indent(d);
   std::cout << ")\n";
}

// Elements

Elements::Elements()
{
}

Elements::~Elements()
{
   //std::cout << "~Elements\n";
}

void Elements::add(Element_p el)
{
   elements.push_back(el);
}

std::size_t Elements::size()
{
   return elements.size();
}

Element_p Elements::get(int i)
{
   return elements[i];
}


// Vector
Vector::Vector()
{
}

Vector::~Vector()
{
   //std::cout << "~Vector\n";
}

void Vector::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Vector:" << type_to_s();

   for (Element_p el: elements)
   {
      el->show(d + 1, chan);
   }
}

void Vector::format(int d)
{
   std::cout << "[";
   for (Element_p el: elements)
   {
      el->format(d + 1);
      std::cout << " ";
   }
   std::cout << "]";
}

Elements_p Vector::make_copy()
{
   return std::make_shared<Vector>();
}


// Body
Body::Body()
{
}

Body::~Body()
{
   //std::cout << "~Body\n";
}

Elements_p Body::make_copy()
{
   return std::make_shared<Body>();
}

void Body::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Body:" << type_to_s();

   for (Element_p el: elements)
   {
      el->show(d + 1, chan);
   }
}

void Body::format(int d)
{
   std::cout << "\n";

   for (Element_p el: elements)
   {
      el->format(d + 1);
      std::cout << "\n";
   }
}

// Binary

Binary::Binary()
{
}

Binary::~Binary()
{
   //std::cout << "~Binary\n";
}

void Binary::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Binary:" << type_to_s();

   for (Element_p el: elements)
   {
      el->show(d + 1, chan);
   }
}

void Binary::format(int d)
{
   std::cout << "(" << info() << " ";

   for (Element_p el: elements)
   {
      el->format(d + 1);
      std::cout << " ";
   }
   std::cout << ")";
}

// Mul

void Mul::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Mul";
   Binary::show(d + 1, chan);
}


// Div

void Div::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Div";
   Binary::show(d + 1, chan);
}


// Plus

void Plus::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Plus";
   Binary::show(d + 1, chan);
}

// Min

void Min::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Min";
   Binary::show(d + 1, chan);
}

// Equal

void Equal::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Equal";
   Binary::show(d + 1, chan);
}

// Less

void Less::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Less";
   Binary::show(d + 1, chan);
}

// Greater

void Greater::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Greater";
   Binary::show(d + 1, chan);
}

// NotEqual

void NotEqual::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "NotEqual";
   Binary::show(d + 1, chan.c_str());
}

// GreaterEq

void GreaterEq::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "GreaterEq";
   Binary::show(d + 1, chan);
}

// LessEq

void LessEq::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "LessEq";
   Binary::show(d + 1, chan);
}

// Defn

Defn::Defn() : fn(nullptr)
{
}

Defn::~Defn()
{
   //std::cout << "~Defn\n";
}

void Defn::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Defn:" << type_to_s() << " " << name << " calls " << calls.size();
   fn->show(d + 1, chan);
}

void Defn::format(int d)
{
   indent(d);
   std::cout << "(defn " << name << "\n";
   fn->format(d);
   indent(d);
   std::cout << ")\n";
}

// Lambda

Lambda::Lambda() : fn(nullptr)
{
}

Lambda::~Lambda()
{
   //std::cout << "~Lambda\n";
}

void Lambda::show(int d, const std::string &chan)
{
   //std::cout << "Lambda::show()\n";
   CLOG(DEBUG, chan.c_str()) << i(d) << "Lambda:" << type_to_s();
   if (fn != nullptr)
   {
       fn->show(d + 1, chan);
   }
}

void Lambda::format(int d)
{
   //indent(d);
   std::cout << "(fn\n";
   fn->format(d + 1);
   indent(d);
   std::cout << ")";
}

// AParam

AParam::AParam() : rest(false), listonly(true), beforeamp(false)
{
}

AParam::~AParam()
{
}


// Param

Param::Param(std::string nm) : name(nm)
{
}

Param::~Param()
{
}

void Param::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Param " << name << " " << rest << " " << beforeamp;
}

void Param::format(int d)
{
   if (getRest())
   {
      std::cout << "& ";
   }
   std::cout << name << " ";
}

void Param::setBeforeAmp(int d, bool val)
{
   CLOG(DEBUG, "parser") << i(d) << "Param setBeforeAmp d " << d;
   if (d > 1 && val)
   {
      CLOG(DEBUG, "parser") << i(d) << "Param setBeforeAmp true";
      beforeamp = true;
   }
}

// ParamList

ParamList::ParamList()
{
}

ParamList::~ParamList()
{
}

void ParamList::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "ParamList " << rest << " " << beforeamp;

   for (AParam_p p: params)
   {
      p->show(d + 1, chan);
   }
}

void ParamList::format(int d)
{
   std::cout << "[";

   for (AParam_p p: params)
   {
      if (p->getRest())
      {
         //std::cout << "& ";
      }
      p->format(d + 1);
   }
   std::cout << "]";
}

// Is there any Param with rest set?
bool ParamList::hasRest()
{
   for (AParam_p p: params)
   {
      if (p->getRest())
      {
         return true;
      }
   }
   return false;
}

// Run through all the sub params
// and set beforeamp according to the following rule:
//
// * Param's is in the root ParamList all get false
//   even if there is an &
// * In non-root ParamList's the Param's before the & get true.
void ParamList::setBeforeAmp(int d, bool val)
{
   CLOG(DEBUG, "parser") << i(d) << "ParamList setBeforeAmp d " << d;
   bool first = true;
   for (AParam_p p: params)
   {
      bool bfa = false;
      if (d >= 1 && hasRest())
      {
         // only non-root ParamList which has an &
         bfa = true;
      }

      // Is this a Param preceded by &?
      if (p->getRest())
      {
         // stop using true as value for beforeamp
         first = false;
      }
      if (first)
      {
         p->setBeforeAmp(d + 1, bfa);
      }
      else
      {
         p->setBeforeAmp(d + 1, false);
      }
   }
}

// Fn

Fn::Fn() : body(nullptr), full(false)
{
}

Fn::~Fn()
{
   //std::cout << "~Fn\n";
}

void Fn::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Fn:" << type_to_s();
   CLOG(DEBUG, chan.c_str()) << i(d + 1) << "full " << full;

   /*
   indent(d + 1);
   std::cout << "params\n";
   for (std::string param: params)
   {
      indent(d + 2);
      std::cout << param << "\n";
   }
    */

   if (paramlist == nullptr)
   {
      std::cout << "fn show paramlist nullptr";
      throw std::make_unique<ParserError>();
   }

   paramlist->show(d + 1, chan);

   if (body == nullptr)
   {
      std::cout << "fn show body nullptr\n";
      throw std::make_unique<ParserError>();
   }

   body->show(d + 1, chan);
}

void Fn::format(int d)
{
   //std::cout << "(fn ";

   if (paramlist == nullptr)
   {
      std::cout << "fn format paramlist nullptr\n";
      throw std::make_unique<ParserError>();
   }

   indent(d);
   paramlist->format(d + 1);
   //std::cout << "\n";

   if (body == nullptr)
   {
      std::cout << "fn format body nullptr\n";
      throw std::make_unique<ParserError>();
   }

   //indent(d);
   body->format(d);
   //std::cout << ")";
}

// Bind

Bind::Bind() : scope(nullptr)
{
}

Bind::~Bind()
{
   //std::cout << "~Bind\n";
}

void Bind::format(int d)
{
}

// If

If::If() : condition(nullptr), yes(nullptr), no(nullptr)
{
}

If::~If()
{
   //std::cout << "~If\n";
}

void If::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "If:" << type_to_s();

   condition->show(d + 1, chan);
   yes->show(d + 1, chan);
   if (no != nullptr)
   {
      no->show(d + 1, chan);
   }
}

void If::format(int d)
{
   indent(d);
   std::cout << "(if\n";

   condition->format(d + 1);
   std::cout << "\n";
   yes->format(d + 1);
   if (no != nullptr)
   {
      no->format(d + 1);
      std::cout << "\n";
   }
   indent(d);
   std::cout << ")";
}

// Println

Println::Println()
{
}

Println::~Println()
{
}

void Println::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Println:" << type_to_s() << " " << full;

   if (body != nullptr)
   {
      body->show(d + 1, chan);
   }
}

void Println::format(int d)
{
   std::cout << "(println ";

   if (body != nullptr)
   {
      body->format(d + 1);
   }
   std::cout << ")\n";
}

// Print

Print::Print()
{
}

Print::~Print()
{
}

void Print::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Print:" << type_to_s() << " " << full;

   if (body != nullptr)
   {
      body->show(d + 1, chan);
   }
}

void Print::format(int d)
{
   std::cout << "(print ";

   if (body != nullptr)
   {
      body->format(d + 1);
   }
   std::cout << ")\n";
}

// Ampersand

Ampersand::Ampersand()
{
}

Ampersand::~Ampersand()
{
}

void Ampersand::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Ampersand:" << type_to_s();
}

void Ampersand::format(int d)
{
   std::cout << "&";
}

// Let

Let::Let()
{
}

Let::~Let()
{
}

void Let::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Let:" << type_to_s() << " " << full;

   for (const auto &pr: variables)
   {
      //indent(d + 1);
      //std::cout << pr.first << "\n";
      pr.first->show(d + 2, chan);

      pr.second->show(d + 2, chan);
   }

   if (body != nullptr)
   {
      body->show(d + 1, chan);
   }
}

void Let::format(int d)
{
   ///indent(d);
   std::cout << "(let\n";

   for (const auto &pr: variables)
   {
      std::cout << pr.first << " ";

      pr.second->format(d + 2);
      std::cout << "\n";
   }

   if (body != nullptr)
   {
      body->format(d + 1);
   }
   ///indent(d);
   std::cout << ")";
}



// Symbol

Symbol::Symbol(const std::string &te) : text(te)
{
}

Symbol::~Symbol()
{
   //std::cout << "~Symbol " << text << "\n";
}

void Symbol::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Symbol:" << type_to_s() << " " << text;
}

void Symbol::format(int d)
{
   std::cout << text;
}


// Builtin

Builtin::Builtin(const std::string &te) : text(te)
{
}

Builtin::~Builtin()
{
   //std::cout << "~Builtin " << text << "\n";
}

void Builtin::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Builtin:" << type_to_s() << " " << text;
}

void Builtin::format(int d)
{
   std::cout << text;
}


// Text

Text::Text(const std::string &te) : text(te)
{
}

Text::~Text()
{
   //std::cout << "~Symbol " << text << "\n";
}

void Text::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Text " << text;
}

void Text::format(int d)
{
   std::cout << "\"" << text << "\"";
}


// Main

Main::Main()
{
}

Main::~Main()
{
   //std::cout << "~Main\n";
}

/*
void Main::add(Element_p el)
{
   elements.push_back(el);
}
 */

void Main::show(int d, const std::string &chan)
{
   CLOG(DEBUG, chan.c_str()) << i(d) << "Main:" << type_to_s();

   CLOG(DEBUG, chan.c_str()) << i(d + 1) << "defines: ";
   for (const auto &pr: defines)
   {
      CLOG(DEBUG, chan.c_str()) << i(d + 2) << pr.first << " " << pr.second;
   }

   for (Element_p el: elements)
   {
      el->show(d + 1, chan);
   }
}

void Main::format(int d)
{
   for (Element_p el: elements)
   {
      el->format(d);
      std::cout << "\n";
   }
}



// parserfuncties

ParamList_p Parser::parameters_rec(Element_p els)
{
   bool rest      = false;  // ampersand detected
   bool restel    = false;  // element after ampersand detected
   ParamList_p pl = std::make_shared<ParamList>();

   Vector_p params = std::dynamic_pointer_cast<Vector>(els);
   if (params == nullptr)
   {
      std::cout << "error in defn/fn: parameter vector missing\n";
      throw std::make_unique<ParserError>();
   }

   for (Element_p ell: params->getElements())
   {
      Symbol_p    param  = nullptr;
      Ampersand_p param2 = nullptr;
      Vector_p    param3 = nullptr;

      param = std::dynamic_pointer_cast<Symbol>(ell);
      if (param != nullptr)
      {
         if (restel)
         {
            std::cout << "error in defn/fn: only 1 parameter after & allowed\n";
            throw std::make_unique<ParserError>();
         }

         Param_p par = std::make_shared<Param>(param->getText());
         pl->addParam(par);

         if (rest)
         {
            restel = true;
            par->setRest(true);
         }
      }
      else
      {
         param2 = std::dynamic_pointer_cast<Ampersand>(ell);
         if (param2 != nullptr)
         {
            CLOG(DEBUG, "parser") << "ampersand detected";
            rest = true;
         }
         else
         {
            param3 = std::dynamic_pointer_cast<Vector>(ell);
            if (param3 != nullptr)
            {
               CLOG(DEBUG, "parser") << "vector parameter detected";

               if (restel)
               {
                  std::cout << "error in defn/fn: only 1 parameter after & allowed\n";
                  throw std::make_unique<ParserError>();
               }

               AParam_p par = parameters_rec(param3);
               pl->addParam(par);

               if (rest)
               {
                  restel = true;
                  par->setRest(true);
                  par->setListonly(false);
               }
            }
            else
            {
               std::cout << "error in defn/fn: parameter name must be symbol\n";
               throw std::make_unique<ParserError>();
            }
         }
      }
   }
   if (rest && !restel)
   {
      std::cout << "error in defn/fn: parameter after & missing\n";
      throw std::make_unique<ParserError>();
   }
   return pl;
}

ParamList_p Parser::parameters(Element_p els)
{
   ParamList_p pars = parameters_rec(els);
   pars->setListonly(false);
   pars->setBeforeAmp(0, false);

   return pars;
}


Element_p Parser::list(bool isliteral)
{
   CLOG(DEBUG, "parser") << "list";

   if (!isliteral)
   {
      Call_p lst = std::make_shared<Call>();

      char token = lex->token();
      if (token != tk_eof)
      {
         if (token == tk_bropen)
         {
            lex->next();

            while (lex->token() != tk_eof && lex->token() != tk_brclose)
            {
               Element_p e = expression();
               lst->add(e);
            }
            if (lex->token() == tk_brclose)
            {
               CLOG(DEBUG, "parser") << "list close ok";
               lex->next();
            }
         }
      }

      if (lst->size() == 0)
      {
         std::cout << "an empty list is useless as call\n";
         throw std::make_unique<ParserError>();
      }

      // rearrange the list
      Element_p el = lst->get(0);
      Binary_p bin = std::dynamic_pointer_cast<Binary>(el);
      if (bin != nullptr)
      {
         int i = 0;
         while (!lst->getElements().empty())
         {
            CLOG(DEBUG, "parser") << "Binary pop " << i;

            Element_p e = lst->getElements().front();
            e->show(0, "parser");

            lst->pop_front();
            if (i != 0)
            {
               bin->add(e);
            }
            i++;
         }

         CLOG(DEBUG, "parser") << "Binary " << bin->info();
         bin->show(0, "parser");

         // test only
         //exit(1);

         return bin;
      }
      else
      {
         Defn_p defn = std::dynamic_pointer_cast<Defn>(el);
         if (defn != nullptr)
         {
            // skip defn
            lst->pop_front();

            CLOG(DEBUG, "parser") << "defn";

            // take the name of the defined function
            Symbol_p name = std::dynamic_pointer_cast<Symbol>(lst->get(0));
            if (name == nullptr)
            {
               std::cout << "error in defn: name missing\n";
               throw std::make_unique<ParserError>();
            }
            CLOG(DEBUG, "parser") << "name " << name->getText();
            defn->setName(name->getText());
            lst->pop_front();


            Fn_p fn = std::make_shared<Fn>();
            ParamList_p parlst = parameters(lst->get(0));
            //parlst->setListonly(false);
            fn->setParamList(parlst);

            /*
            Vector_p params = std::dynamic_pointer_cast<Vector>(lst->get(0));
            if (params == nullptr)
            {
               std::cout << "error in defn: parameter vector missing\n";
               throw std::make_unique<ParserError>();
            }

            Fn_p fn = std::make_shared<Fn>();
            defn->setFn(fn);

            for (Element_p ell: params->getElements())
            {
               Symbol_p param = std::dynamic_pointer_cast<Symbol>(ell);
               if (param == nullptr)
               {
                  std::cout << "error in defn: parameter name must be symbol\n";
                  throw std::make_unique<ParserError>();
               }
               fn->addParam(param->getText());
            }
             */

            // skip parameter vector
            lst->pop_front();

            defn->setFn(fn);

            Body_p bd = std::make_shared<Body>();
            while (!lst->getElements().empty())
            {
               Element_p e = lst->getElements().front();
               lst->pop_front();
               bd->add(e);
            }
            fn->setBody(bd);

            return defn;
         }
         else
         {
            Fn_p fn = std::dynamic_pointer_cast<Fn>(el);
            if (fn != nullptr && !fn->isFull())
            {
               lst->pop_front();

               CLOG(DEBUG, "parser") << "fn";
               lst->show(0, "parser");
               lst->get(0)->show(0, "parser");

               ParamList_p parlst = parameters(lst->get(0));
               // parlst->setListonly(false);
               fn->setParamList(parlst);

               /*
               Vector_p params = std::dynamic_pointer_cast<Vector>(lst->get(0));
               if (params == nullptr)
               {
                  std::cout << "error in fn: parameter vector missing\n";
                  throw std::make_unique<ParserError>();
               }
               if (debug) std::cout << "parse fn parameters ok\n";

               for (Element_p ell: params->getElements())
               {
                  Symbol_p param = std::dynamic_pointer_cast<Symbol>(ell);
                  if (param == nullptr)
                  {
                     std::cout << "error in fn: parameter name must be symbol\n";
                     throw std::make_unique<ParserError>();
                  }
                  fn->addParam(param->getText());
               }
                */

               // skip parameter vector
               lst->pop_front();

               Body_p bd = std::make_shared<Body>();
               while (!lst->getElements().empty())
               {
                  Element_p e = lst->getElements().front();
                  lst->pop_front();
                  bd->add(e);
               }
               fn->setBody(bd);
               fn->setFull(true);
               fn->show(0, "parser");

               Lambda_p la = std::make_shared<Lambda>();
               la->setFn(fn);

               return la;
            }
            else
            {
               // only for test full
               if (fn == nullptr)
               {
                  CLOG(DEBUG, "parser") << "fn null";
               }
               else
               {
                  // this else is never executed,
                  // so the full flag can be omitted
                  CLOG(DEBUG, "parser") << "fn full";
               }

               If_p fi = std::dynamic_pointer_cast<If>(el);
               if (fi != nullptr)
               {
                  lst->pop_front();

                  CLOG(DEBUG, "parser") << "if";

                  int sz = lst->getElements().size();
                  if (sz != 3 && sz != 2)
                  {
                     std::cout << "error in if: only 1 or 2 actions allowed, sz " << sz << "\n";
                     throw std::make_unique<ParserError>();
                  }

                  Element_p e1 = lst->getElements().front();
                  lst->pop_front();
                  fi->setCondition(e1);
                  CLOG(DEBUG, "parser") << "if condtion";
                  e1->show(0, "parser");

                  Element_p e2 = lst->getElements().front();
                  lst->pop_front();
                  fi->setYes(e2);

                  if (sz == 3)
                  {
                     Element_p e3 = lst->getElements().front();
                     lst->pop_front();
                     fi->setNo(e3);
                  }

                  return fi;
               }
               else
               {
                  Println_p priln = std::dynamic_pointer_cast<Println>(el);
                  if (priln != nullptr)
                  {
                     lst->pop_front();

                     CLOG(DEBUG, "parser") << "println";
                     lst->show(0, "parser");

                     Body_p bd = std::make_shared<Body>();
                     while (!lst->getElements().empty())
                     {
                        Element_p e = lst->getElements().front();
                        lst->pop_front();
                        bd->add(e);
                     }
                     priln->setBody(bd);
                     priln->show(0, "parser");

                     return priln;
                  }
                  else
                  {
                     Print_p pri = std::dynamic_pointer_cast<Print>(el);
                     if (pri != nullptr)
                     {
                        lst->pop_front();
   
                        CLOG(DEBUG, "parser") << "print";
                        lst->show(0, "parser");
   
                        Body_p bd = std::make_shared<Body>();
                        while (!lst->getElements().empty())
                        {
                           Element_p e = lst->getElements().front();
                           lst->pop_front();
                           bd->add(e);
                        }
                        pri->setBody(bd);
                        pri->show(0, "parser");
   
                        return pri;
                     }
                     else
                     {
                        Let_p lt = std::dynamic_pointer_cast<Let>(el);
                        if (lt != nullptr)
                        {
                           lst->pop_front();
   
                           CLOG(DEBUG, "parser") << "let";
                           lst->show(0, "parser");
                           lst->get(0)->show(0, "parser");
   
                           Vector_p vals = std::dynamic_pointer_cast<Vector>(lst->get(0));
                           if (vals == nullptr)
                           {
                              std::cout << "error in let: name,value vector missing\n";
                              throw std::make_unique<ParserError>();
                           }
   
                           CLOG(DEBUG, "parser") << "let vals size " << vals->size();
                           if ((vals->size() % 2) != 0)
                           {
                              std::cout << "error in let: name,value number not even\n";
                              throw std::make_unique<ParserError>();
                           }
                           CLOG(DEBUG, "parser") << "let name,value ok";
   
                           for (int i=0; i<vals->size()/2; i++)
                           {
                              CLOG(DEBUG, "parser") << "for vals " << i;
                              /*
                               * old code
                              Symbol_p name = std::dynamic_pointer_cast<Symbol>(vals->get(i));
                              if (name == nullptr)
                              {
                                 std::cout << "error in let: value name must be symbol\n";
                                 throw std::make_unique<ParserError>();
                              }
                              lt->addVariable(name->getText(), vals->get(i + 1));
                               */
                              Symbol_p sym = std::dynamic_pointer_cast<Symbol>(vals->get(2*i));
                              Vector_p vec = std::dynamic_pointer_cast<Vector>(vals->get(2*i));
                              if (sym == nullptr)
                              {
                                 if (vec == nullptr)
                                 {
                                    std::cout << "error in let: value name must be symbol or vector\n";
                                    CLOG(DEBUG, "parser") << "vals get " << i;
                                    vals->get(i)->show(0, "parser");
                                    throw std::make_unique<ParserError>();
                                 }
                              }

                              CLOG(DEBUG, "parser") << "name is symbol or vector";

                              // Ok, we have a symbol or a vector.
                              // Encapsulate this single symbol or vector
                              // into a new vector in order to parse it.
                              Vector_p valname = std::make_shared<Vector>();
                              valname->add(vals->get(2*i));
                              valname->show(0, "parser");


                              // parse
                              // parameters() expects a vector.
                              AParam_p par = parameters(valname);
                              lt->addVariable(par, vals->get(2*i + 1));
                           }
                           lst->pop_front();

                           Body_p bd = std::make_shared<Body>();
                           while (!lst->getElements().empty())
                           {
                              Element_p e = lst->getElements().front();
                              lst->pop_front();
                              bd->add(e);
                           }
                           lt->setBody(bd);
                           lt->show(0, "parser");

                           return lt;
                        }
                        else
                        {
                           return lst;
                        }
                     }
                  }
               }//
            }
         }
      }
   }
   else
   {
      List_p lst = std::make_shared<List>();

      char token = lex->token();
      if (token != tk_eof)
      {
         if (token == tk_bropen)
         {
            lex->next();

            while (lex->token() != tk_eof && lex->token() != tk_brclose)
            {
               Element_p e = expression();
               lst->add(e);
            }
            if (lex->token() == tk_brclose)
            {
               CLOG(DEBUG, "parser") << "literal list close ok";
               lex->next();
            }
         }
      }

      return lst;
   }
}

Element_p Parser::vector()
{
   CLOG(DEBUG, "parser") << "vector";

   char token = lex->token();
   if (token != tk_eof)
   {
      if (token == tk_rbopen)
      {
         lex->next();

         Vector_p vec = std::make_shared<Vector>();
         while (lex->token() != tk_eof && lex->token() != tk_rbclose)
         {
            Element_p e = expression();
            vec->add(e);
         }
         if (lex->token() == tk_rbclose)
         {
            CLOG(DEBUG, "parser") << "vector close ok";
            lex->next();
         }

         return vec;
      }
   }
   else
   {
      std::cout << "eof in vector()\n";
      throw std::make_unique<ParserError>();
   }
   return nullptr;
}

Element_p Parser::expression(bool isliteral)
{
   char token = lex->token();
   if (token == tk_bropen)
   {
      return list(isliteral);
   }
   else
   if (token == tk_rbopen)
   {
      return vector();
   }
   else
   if (token == tk_number)
   {
      number_t val = lex->value();
      lex->next();
      Number_p nu = std::make_shared<Number>(val);
      return nu;
   }
   else
   if (token == tk_true)
   {
      lex->next();
      Boolean_p bo = std::make_shared<Boolean>(true);
      return bo;
   }
   else
   if (token == tk_false)
   {
      lex->next();
      Boolean_p bo = std::make_shared<Boolean>(false);
      return bo;
   }
   else
   if (token == tk_nil)
   {
      lex->next();
      Nil_p ni = std::make_shared<Nil>();
      return ni;
   }
   else
   if (token == tk_plus)
   {
      lex->next();
      return std::make_shared<Plus>();
   }
   else
   if (token == tk_minus)
   {
      lex->next();
      return std::make_shared<Min>();
   }
   else
   if (token == tk_mul)
   {
      lex->next();
      return std::make_shared<Mul>();
   }
   else
   if (token == tk_div)
   {
      lex->next();
      return std::make_shared<Div>();
   }
   else
   if (token == tk_equal)
   {
      lex->next();
      return std::make_shared<Equal>();
   }
   else
   if (token == tk_notequal)
   {
      lex->next();
      return std::make_shared<NotEqual>();
   }
   else
   if (token == tk_less)
   {
      lex->next();
      return std::make_shared<Less>();
   }
   else
   if (token == tk_lesseq)
   {
      lex->next();
      return std::make_shared<LessEq>();
   }
   else
   if (token == tk_greater)
   {
      lex->next();
      return std::make_shared<Greater>();
   }
   else
   if (token == tk_greatereq)
   {
      lex->next();
      return std::make_shared<GreaterEq>();
   }
   else
   if (token == tk_defn)
   {
      lex->next();
      return std::make_shared<Defn>();
   }
   else
   if (token == tk_fn)
   {
      CLOG(DEBUG, "parser") << "expression tk_fn";

      lex->next();
      return std::make_shared<Fn>();
   }
   else
   if (token == tk_if)
   {
      lex->next();
      return std::make_shared<If>();
   }
   else
   if (token == tk_println)
   {
      lex->next();
      return std::make_shared<Println>();
   }
   else
   if (token == tk_print)
   {
      lex->next();
      return std::make_shared<Print>();
   }
   else
   if (token == tk_let)
   {
      lex->next();
      return std::make_shared<Let>();
   }
   else
   if (token == tk_ampersand)
   {
      lex->next();
      return std::make_shared<Ampersand>();
   }
   else
   if (token == tk_symbol)
   {
      std::string sym = lex->text();
      lex->next();
      return std::make_shared<Symbol>(sym);
   }
   else
   if (token == tk_builtin)
   {
      std::string sym = lex->text();
      lex->next();
      return std::make_shared<Builtin>(sym);
   }
   else
   if (token == tk_text)
   {
      std::string text = lex->text();
      lex->next();
      return std::make_shared<Text>(text);
   }
   else
   if (token == tk_apostrophe)
   {
      lex->next();

      Element_p el = expression(true);
      if (el == nullptr)
      {
         std::cout << "null after quote\n";
         throw std::make_unique<ParserError>();
      }

      return el;
   }
   else
   {
      std::cout << "unexpected token in expression\n";
      throw std::make_unique<ParserError>();
   }
}


Element_p Parser::main()
{
   Main_p mn = std::make_shared<Main>();
   int i = 0;
   while (lex->token() != tk_eof)
   {
      CLOG(DEBUG, "parser") << "main " << i;

      Element_p el = expression();
      if (el == nullptr)
      {
         std::cout << "null in main()\n";
         throw std::make_unique<ParserError>();
      }
      mn->add(el);
      i++;
   }

   return mn;
}

Parser::Parser() : lex(nullptr)
{
}

Parser::~Parser()
{
}

Element_p Parser::parse(std::string fn)
{
   CLOG(DEBUG, "parser") << "start";
   lex = std::make_shared<Lex>(fn);

   CLOG(DEBUG, "parser") << "lex_init ok";
   return main();
}
