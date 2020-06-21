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


Number::Number(number_t nu)
{
   number = nu;
}


Number::~Number()
{
   std::cout << "~Number " << number << "\n";
}

void Number::show(int d)
{
   indent(d);
   std::cout << "Number " << number << "\n";
}

// List

List::List()
{
}

List::~List()
{
   std::cout << "~List\n";
   for (Element *el: elements)
   {
      delete el;
   }
}

void List::add(Element *el)
{
   elements.push_back(el);
}

std::size_t List::size()
{
   return elements.size();
}

Element *List::get(int i)
{
   return elements[i];
}

void List::show(int d)
{
   indent(d);
   std::cout << "List\n";

   for (Element *el: elements)
   {
      el->show(d + 1);
   }
}

// Vector
Vector::Vector()
{
}

Vector::~Vector()
{
   std::cout << "~Vector\n";
   for (Element *el: elements)
   {
      delete el;
   }
}

void Vector::add(Element *el)
{
   elements.push_back(el);
}

std::size_t Vector::size()
{
   return elements.size();
}

Element *Vector::get(int i)
{
   return elements[i];
}

void Vector::show(int d)
{
   indent(d);
   std::cout << "Vector\n";

   for (Element *el: elements)
   {
      el->show(d + 1);
   }
}

// Body
Body::Body()
{
}

Body::~Body()
{
   std::cout << "~Body\n";
   for (Element *el: elements)
   {
      delete el;
   }
}

void Body::add(Element *el)
{
   elements.push_back(el);
}

std::size_t Body::size()
{
   return elements.size();
}

Element *Body::get(int i)
{
   return elements[i];
}

void Body::show(int d)
{
   indent(d);
   std::cout << "Body\n";

   for (Element *el: elements)
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
   std::cout << "~Binary\n";
   for (Element *el: elements)
   {
      delete el;
   }
}

void Binary::add(Element *el)
{
   elements.push_back(el);
}

void Binary::show(int d)
{
   indent(d);
   std::cout << "Binary\n";

   for (Element *el: elements)
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
   std::cout << "~Defn\n";
   delete fn;
}

void Defn::show(int d)
{
   indent(d);
   std::cout << "Defn " << name << "\n";
   fn->show(d + 1);
}

// Fn

Fn::Fn() : body(nullptr)
{
}

Fn::~Fn()
{
   std::cout << "~Fn\n";
   delete body;
}

void Fn::show(int d)
{
   indent(d);
   std::cout << "Fn\n";

   indent(d + 1);
   std::cout << "params\n";
   for (std::string param: params)
   {
      indent(d + 2);
      std::cout << param << "\n";
   }

   body->show(d + 1);
}


// If

If::If() : condition(nullptr), yes(nullptr), no(nullptr)
{
}

If::~If()
{
   std::cout << "~If\n";

   delete condition;
   delete yes;
   if (no != nullptr)
   {
      delete no;
   }
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


// Symbol

Symbol::Symbol(const std::string &te) : text(te)
{
}

Symbol::~Symbol()
{
   std::cout << "~Symbol " << text << "\n";
}

void Symbol::show(int d)
{
   indent(d);
   std::cout << "Symbol " << text << "\n";
}


// Main

Main::Main()
{
}

Main::~Main()
{
   std::cout << "~Main\n";
   for (Element *el: elements)
   {
      delete el;
   }
}

void Main::add(Element *el)
{
   elements.push_back(el);
}

void Main::show(int d)
{
   indent(d);
   std::cout << "Main\n";

   for (Element *el: elements)
   {
      el->show(d + 1);
   }
}



// parserfuncties


Element *Parser::list(bool isliteral)
{
   std::cout << "parse list\n";

   List *lst = new List();
   
   char token = lex->token();
   if (token != tk_eof)
   {
      if (token == tk_bropen)
      {
         lex->next();
         
         while (lex->token() != tk_eof && lex->token() != tk_brclose)
         {
            Element *e = expression();
            lst->add(e);
         }
         if (lex->token() == tk_brclose)
         {
            std::cout << "parse list close ok\n";
            lex->next();
         }
      }
   }

   if (!isliteral && lst->size() > 0)
   {
      // rearrange the list
      Element *el = lst->get(0);
      Binary *bin = dynamic_cast<Binary *>(el);
      if (bin != nullptr)
      {
         int i = 0;
         while (!lst->getElements().empty())
         {
            //std::cout << "pop " << lst->getElements().size() << "\n";
            
            Element *e = lst->getElements().front();
            lst->pop_front();
            if (i != 0)
            {
               bin->add(e);
            }
            i++;
         }
         delete lst;
         
         return bin;
      }
      else
      {
         Defn *defn = dynamic_cast<Defn *>(el);
         if (defn != nullptr)
         {
            lst->pop_front();
            
            Symbol *name = dynamic_cast<Symbol *>(lst->get(0));
            if (name == nullptr)
            {
               std::cout << "error in defn: name missing\n";
               throw new ParserError();
            }
            std::cout << "name " << name->getText();
            defn->setName(name->getText());
            lst->pop_front();
            delete name;

            Vector *params = dynamic_cast<Vector *>(lst->get(0));
            if (params == nullptr)
            {
               std::cout << "error in defn: parameter vector missing\n";
               throw new ParserError();
            }

            Fn *fn = new Fn();
            defn->setFn(fn);
            
            for (Element *ell: params->getElements())
            {
               Symbol *param = dynamic_cast<Symbol *>(ell);
               if (param == nullptr)
               {
                  std::cout << "error in defn: parameter name must be symbol\n";
                  throw new ParserError();
               }
               fn->addParam(param->getText());
            }
            lst->pop_front();
            delete params;

            Body *bd = new Body();
            while (!lst->getElements().empty())
            {
               Element *e = lst->getElements().front();
               lst->pop_front();
               bd->add(e);
            }
            fn->setBody(bd);            
            delete lst;
            
            return defn;
         }
         else
         {
            Fn *fn = dynamic_cast<Fn *>(el);
            if (fn != nullptr)
            {
               lst->pop_front();
   
               Vector *params = dynamic_cast<Vector *>(lst->get(0));
               if (params == nullptr)
               {
                  std::cout << "error in fn: parameter vector missing\n";
                  throw new ParserError();
               }
   
               for (Element *ell: params->getElements())
               {
                  Symbol *param = dynamic_cast<Symbol *>(ell);
                  if (param == nullptr)
                  {
                     std::cout << "error in fn: parameter name must be symbol\n";
                     throw new ParserError();
                  }
                  fn->addParam(param->getText());
               }
               lst->pop_front();
               delete params;
               
               Body *bd = new Body();
               while (!lst->getElements().empty())
               {
                  Element *e = lst->getElements().front();
                  lst->pop_front();
                  bd->add(e);
               }
               fn->setBody(bd);            
               delete lst;
               
               return fn;
            }
            else
            {
               If *fi = dynamic_cast<If *>(el);
               if (fi != nullptr)
               {
                  lst->pop_front();
      
                  int sz = lst->getElements().size();
                  if (sz != 3 && sz != 2)
                  {
                     std::cout << "error in if: only 1 or 2 actions allowed, sz " << sz << "\n";
                     throw new ParserError();
                  }
                  
                  Element *e1 = lst->getElements().front();
                  lst->pop_front();
                  fi->setCondition(e1);

                  Element *e2 = lst->getElements().front();
                  lst->pop_front();
                  fi->setYes(e2);

                  if (sz == 3)
                  {
                     Element *e3 = lst->getElements().front();
                     lst->pop_front();
                     fi->setNo(e3);
                  }
                  delete lst;
                  
                  return fi;
               }
               else
               {
                  return lst;
               }
            }
         }
      }
   }
   else
   {
      return lst;
   }
}

Element *Parser::vector()
{
   std::cout << "parse vector\n";
   
   char token = lex->token();
   if (token != tk_eof)
   {
      if (token == tk_rbopen)
      {
         lex->next();
         
         Vector *vec = new Vector();
         while (lex->token() != tk_eof && lex->token() != tk_rbclose)
         {
            Element *e = expression();
            vec->add(e);
         }
         if (lex->token() == tk_rbclose)
         {
            std::cout << "parse vector close ok\n";
            lex->next();
         }
         
         return vec;
      }
   }
   else
   {
      std::cout << "eof in vector()\n";
      throw new ParserError();
   }
}

Element *Parser::expression(bool isliteral)
{
   /*
   if (lex->token() == tk_eof)
   {
      return nullptr;
   }
    */
   
   char token = lex->token();
   if (token == tk_bropen)
   {
      return list();
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
      Number *nu = new Number(val);
      return nu;
   }
   else
   if (token == tk_plus)
   {
      lex->next();
      return new Plus();
   }
   else
   if (token == tk_minus)
   {
      lex->next();
      return new Min();
   }
   else
   if (token == tk_mul)
   {
      lex->next();
      return new Mul();
   }
   else
   if (token == tk_div)
   {
      lex->next();
      return new Div();
   }
   else
   if (token == tk_equal)
   {
      lex->next();
      return new Equal();
   }
   else
   if (token == tk_notequal)
   {
      lex->next();
      return new NotEqual();
   }
   else
   if (token == tk_less)
   {
      lex->next();
      return new Less();
   }
   else
   if (token == tk_lesseq)
   {
      lex->next();
      return new LessEq();
   }
   else
   if (token == tk_greater)
   {
      lex->next();
      return new Greater();
   }
   else
   if (token == tk_greatereq)
   {
      lex->next();
      return new GreaterEq();
   }
   else
   if (token == tk_defn)
   {
      lex->next();
      return new Defn();
   }
   else
   if (token == tk_fn)
   {
      lex->next();
      return new Fn();
   }
   else
   if (token == tk_if)
   {
      lex->next();
      return new If();
   }
   else
   if (token == tk_symbol)
   {
      std::string sym = lex->text();
      lex->next();
      return new Symbol(sym);
   }
   else
   {
      std::cout << "unexpected token in expression\n";
      throw new ParserError();
   }
}


Element *Parser::main()
{
   Main *mn = new Main();
   int i = 0;
   while (lex->token() != tk_eof)
   {
      std::cout << "main " << i << "\n";
      
      Element *el = expression();
      if (el == nullptr)
      {
         std::cout << "null in main()\n";
         throw new ParserError();
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
   if (lex != nullptr)
   {
      delete lex;
   }
}

Element *Parser::parse(std::string fn)
{
   printf("start parser\n");
   lex = new Lex(fn);
   
   printf("lex_init ok\n");
   return main();
}
