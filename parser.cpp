#include <iostream>

#include "textfile.h"
#include "parser.h"


void indent(int d)
{
   while (d-- > 0)
   {
      printf("   ");
   }
}

Element::Element()
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

void Number::show(int d)
{
   indent(d);
   std::cout << "Number " << number << "\n";
}

// Boolean

Boolean::Boolean(bool val) : value(val)
{
}

Boolean::~Boolean()
{
   //std::cout << "~Boolean " << value << "\n";
}

void Boolean::show(int d)
{
   indent(d);
   std::cout << "Boolean " << value << "\n";
}


// Nil

Nil::Nil()
{
}

Nil::~Nil()
{
}

void Nil::show(int d)
{
   indent(d);
   std::cout << "Nil\n";
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

std::size_t List::size()
{
   return elements.size();
}

Element_p List::get(int i)
{
   return elements[i];
}

void List::show(int d)
{
   indent(d);
   std::cout << "List\n";

   for (Element_p el: elements)
   {
      el->show(d + 1);
   }
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

void Call::show(int d)
{
   indent(d);
   std::cout << "Call\n";

   for (Element_p el: elements)
   {
      el->show(d + 1);
   }
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

void Vector::show(int d)
{
   indent(d);
   std::cout << "Vector\n";

   for (Element_p el: elements)
   {
      el->show(d + 1);
   }
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

void Body::show(int d)
{
   indent(d);
   std::cout << "Body\n";

   for (Element_p el: elements)
   {
      el->show(d + 1);
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

void Binary::show(int d)
{
   indent(d);
   std::cout << "Binary\n";

   for (Element_p el: elements)
   {
      el->show(d + 1);
   }
}

// Mul

void Mul::show(int d)
{
   indent(d);
   std::cout << "Mul\n";
   Binary::show(d + 1);
}


// Div

void Div::show(int d)
{
   indent(d);
   std::cout << "Div\n";
   Binary::show(d + 1);
}


// Plus

void Plus::show(int d)
{
   indent(d);
   std::cout << "Plus\n";
   Binary::show(d + 1);
}

// Min

void Min::show(int d)
{
   indent(d);
   std::cout << "Min\n";
   Binary::show(d + 1);
}

// Equal

void Equal::show(int d)
{
   indent(d);
   std::cout << "Equal\n";
   Binary::show(d + 1);
}

// Less

void Less::show(int d)
{
   indent(d);
   std::cout << "Less\n";
   Binary::show(d + 1);
}

// Greater

void Greater::show(int d)
{
   indent(d);
   std::cout << "Greater\n";
   Binary::show(d + 1);
}

// NotEqual

void NotEqual::show(int d)
{
   indent(d);
   std::cout << "NotEqual\n";
   Binary::show(d + 1);
}

// GreaterEq

void GreaterEq::show(int d)
{
   indent(d);
   std::cout << "GreaterEq\n";
   Binary::show(d + 1);
}

// LessEq

void LessEq::show(int d)
{
   indent(d);
   std::cout << "LessEq\n";
   Binary::show(d + 1);
}

// Defn

Defn::Defn() : fn(nullptr)
{
}

Defn::~Defn()
{
   //std::cout << "~Defn\n";
}

void Defn::show(int d)
{
   indent(d);
   std::cout << "Defn " << name << "\n";
   fn->show(d + 1);
}

// Lambda

Lambda::Lambda() : fn(nullptr)
{
}

Lambda::~Lambda()
{
   //std::cout << "~Lambda\n";
}

void Lambda::show(int d)
{
   indent(d);
   std::cout << "Lambda\n";
   fn->show(d + 1);
}

// AParam

AParam::AParam() : rest(false)
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

void Param::show(int d)
{
   indent(d);
   std::cout << "Param " << name << " " << rest << "\n";
}

// ParamList

ParamList::ParamList()
{
}

ParamList::~ParamList()
{
}

void ParamList::show(int d)
{
   indent(d);
   std::cout << "ParamList " << rest << "\n";
   
   for (AParam_p p: params)
   {
      p->show(d + 1);
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

void Fn::show(int d)
{
   indent(d);
   std::cout << "Fn\n";

   indent(d + 1);
   std::cout << "full " << full <<"\n";

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
      std::cout << "fn show paramlist nullptr\n";
      throw std::make_unique<ParserError>();
   }

   paramlist->show(d + 1);

   if (body == nullptr)
   {
      std::cout << "fn show body nullptr\n";
      throw std::make_unique<ParserError>();
   }

   body->show(d + 1);
}

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

   paramlist->assignParameters(cx, fr, list, d, false);
   return true;
}

// Bind

Bind::Bind() : lambda(nullptr)
{
}

Bind::~Bind()
{
   //std::cout << "~Bind\n";
}


// If

If::If() : condition(nullptr), yes(nullptr), no(nullptr)
{
}

If::~If()
{
   //std::cout << "~If\n";
}

void If::show(int d)
{
   indent(d);
   std::cout << "If\n";
   
   condition->show(d + 1);
   yes->show(d + 1);
   if (no != nullptr)
   {
      no->show(d + 1);
   }
}

// Println

Println::Println()
{
}

Println::~Println()
{
}

void Println::show(int d)
{
   indent(d);
   std::cout << "Println " << full << "\n";
   
   if (body != nullptr)
   {
      body->show(d + 1);
   }
}

// Ampersand

Ampersand::Ampersand()
{
}

Ampersand::~Ampersand()
{
}

void Ampersand::show(int d)
{
   indent(d);
   std::cout << "Ampersand\n";
}

// Let

Let::Let()
{
}

Let::~Let()
{
}

void Let::show(int d)
{
   indent(d);
   std::cout << "Let " << full << "\n";
   
   for (const auto &pr: variables)
   {
      indent(d + 1);
      std::cout << pr.first << "\n";
      
      pr.second->show(d + 2);
   }

   if (body != nullptr)
   {
      body->show(d + 1);
   }
}



// Symbol

Symbol::Symbol(const std::string &te) : text(te)
{
}

Symbol::~Symbol()
{
   //std::cout << "~Symbol " << text << "\n";
}

void Symbol::show(int d)
{
   indent(d);
   std::cout << "Symbol " << text << "\n";
}


// Text

Text::Text(const std::string &te) : text(te)
{
}

Text::~Text()
{
   //std::cout << "~Symbol " << text << "\n";
}

void Text::show(int d)
{
   indent(d);
   std::cout << "Text " << text << "\n";
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

void Main::show(int d)
{
   indent(d);
   std::cout << "Main\n";

   for (Element_p el: elements)
   {
      el->show(d + 1);
   }
}



// parserfuncties

ParamList_p Parser::parameters(Element_p els)
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
            std::cout << "ampersand detected\n";
            rest = true;
         }
         else
         {
            param3 = std::dynamic_pointer_cast<Vector>(ell);
            if (param3 != nullptr)
            {
               std::cout << "vector parameter detected\n";
      
               if (restel)
               {
                  std::cout << "error in defn/fn: only 1 parameter after & allowed\n";
                  throw std::make_unique<ParserError>();
               }

               AParam_p par = parameters(param3);
               pl->addParam(par);
               
               if (rest)
               {
                  restel = true;
                  par->setRest(true);
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

Element_p Parser::list(bool isliteral)
{
   if (debug) std::cout << "parse list\n";

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
               if (debug) std::cout << "parse list close ok\n";
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
            if (debug) std::cout << "parse Binary pop " << i << "\n";
            
            Element_p e = lst->getElements().front();
            if (debug) e->show(0);

            lst->pop_front();
            if (i != 0)
            {
               bin->add(e);
            }
            i++;
         }

         if (debug) std::cout << "parse Binary " << bin->info() << "\n";
         if (debug) bin->show(0);
         
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

            if (debug) std::cout << "parse defn\n";
            
            // take the name of the defined function
            Symbol_p name = std::dynamic_pointer_cast<Symbol>(lst->get(0));
            if (name == nullptr)
            {
               std::cout << "error in defn: name missing\n";
               throw std::make_unique<ParserError>();
            }
            if (debug) std::cout << "name " << name->getText();
            defn->setName(name->getText());
            lst->pop_front();

            
            Fn_p fn = std::make_shared<Fn>();
            ParamList_p parlst = parameters(lst->get(0));
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
   
               if (debug) std::cout << "parse fn\n";
               if (debug) lst->show(0);
               if (debug) lst->get(0)->show(0);

               ParamList_p parlst = parameters(lst->get(0));
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
               if (debug) fn->show(0);
               
               Lambda_p la = std::make_shared<Lambda>();
               la->setFn(fn);
               
               return la;
            }
            else
            {
               // only for test full
               if (fn == nullptr)
               {
                  if (debug) std::cout << "parse fn null\n";
               }
               else
               {
                  if (debug) std::cout << "parse fn full\n";
               }

               If_p fi = std::dynamic_pointer_cast<If>(el);
               if (fi != nullptr)
               {
                  lst->pop_front();
      
                  if (debug) std::cout << "parse if\n";

                  int sz = lst->getElements().size();
                  if (sz != 3 && sz != 2)
                  {
                     std::cout << "error in if: only 1 or 2 actions allowed, sz " << sz << "\n";
                     throw std::make_unique<ParserError>();
                  }
                  
                  Element_p e1 = lst->getElements().front();
                  lst->pop_front();
                  fi->setCondition(e1);
                  if (debug) std::cout << "parse if condtion\n";
                  if (debug) e1->show(0);

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
                  Println_p pri = std::dynamic_pointer_cast<Println>(el);
                  if (pri != nullptr)
                  {
                     lst->pop_front();
                     
                     if (debug) std::cout << "parse println\n";
                     if (debug) lst->show(0);
                     
                     Body_p bd = std::make_shared<Body>();
                     while (!lst->getElements().empty())
                     {
                        Element_p e = lst->getElements().front();
                        lst->pop_front();
                        bd->add(e);
                     }
                     pri->setBody(bd);
                     if (debug) pri->show(0);
                     
                     return pri;
                  }
                  else
                  {
                     Let_p lt = std::dynamic_pointer_cast<Let>(el);
                     if (lt != nullptr)
                     {
                        lst->pop_front();
                        
                        if (debug) std::cout << "parse let\n";
                        if (debug) lst->show(0);
                        if (debug) lst->get(0)->show(0);

                        Vector_p vals = std::dynamic_pointer_cast<Vector>(lst->get(0));
                        if (vals == nullptr)
                        {
                           std::cout << "error in let: name,value vector missing\n";
                           throw std::make_unique<ParserError>();
                        }

                        if (debug) std::cout << "parse let vals size " << vals->size() << "\n";
                        if ((vals->size() % 2) != 0)
                        {
                           std::cout << "error in let: name,value number not even\n";
                           throw std::make_unique<ParserError>();
                        }
                        if (debug) std::cout << "parse let name,value ok\n";
                        
                        for (int i=0; i<vals->size()/2; i++)
                        {
                           Symbol_p name = std::dynamic_pointer_cast<Symbol>(vals->get(i));
                           if (name == nullptr)
                           {
                              std::cout << "error in let: value name must be symbol\n";
                              throw std::make_unique<ParserError>();
                           }
                           lt->addVariable(name->getText(), vals->get(i + 1));
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
                        if (debug) lt->show(0);
                        
                        return lt;
                     }
                     else
                     {
                        return lst;
                     }
                  }
               }
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
               if (debug) std::cout << "parse literal list close ok\n";
               lex->next();
            }
         }
      }

      return lst;
   }
}

Element_p Parser::vector()
{
   if (debug) std::cout << "parse vector\n";
   
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
            if (debug) std::cout << "parse vector close ok\n";
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
      if (debug) std::cout << "parse expression tk_fn\n";
      
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
      if (debug) std::cout << "main " << i << "\n";
      
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
   if (debug) printf("start parser\n");
   lex = std::make_shared<Lex>(fn);
   
   if (debug) printf("lex_init ok\n");
   return main();
}
