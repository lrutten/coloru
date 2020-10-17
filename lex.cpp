#include <iostream>
#include <sstream>
#include <string.h>
#include "lex.h"

extern bool debug;

token_t Lex::token()
{
   return tok;
}

number_t Lex::value()
{
   return val;
}
  
Lex::Lex(const std::string &fn) : file(std::make_shared<Textfile>(fn)), it(file->begin())
{
   next();
}

Lex::~Lex()
{
   //delete file;
}

int Lex::charnr()
{
   return 0;
}

int isdigit(char t)
{
   return (t >= '0' && t <= '9');
}

int isletter(char t)
{
   return (t >= 'a' && t <= 'z' || t >= 'A' && t <= 'Z');
}

bool isextra(char t)
{
   return t == '*' || t == '+' || t == '-' || t == '!' || t == '?' || t == '_';
}

bool Lex::end()
{
   return it == file->end();
}

void Lex::next()
{
   while (!end() && (*it == ' ' || *it == '\n'))
   {
      it++;
   }
   
   if (end())
   {
      tok = tk_eof;
      return;
   }
   if (debug) std::cout << "teken " << *it << "\n";

   switch ( *it )
   {
      case '(':
         tok = tk_bropen;
         it++;
         break;
      case ')':
         tok = tk_brclose;
         it++;
         break;
      case '[':
         tok = tk_rbopen;
         it++;
         break;
      case ']':
         tok = tk_rbclose;
         it++;
         break;
      case '*':
         tok = tk_mul;
         it++;
         break;
      case '-':
         tok = tk_minus;
         it++;
         break;
      case '+':
         tok = tk_plus;
         it++;
         break;
      case '/':
         tok = tk_div;
         it++;
         break;
      case '&':
         tok = tk_ampersand;
         it++;
         break;
      case '\'':
         tok = tk_apostrophe;
         it++;
         break;
      case '=':
         tok = tk_equal;
         it++;
         break;
      case '<':
         tok = tk_less;
         it++;
         break;
      case '>':
         tok = tk_greater;
         it++;
         break;
      
      default:
         if (isdigit(*it))
         {
            std::string nus = "";
            while (!end() && isdigit(*it))
            {
               nus += *it;
               it++;
            }
            tok = tk_number;
            std::stringstream ss(nus);
            ss >> val;
            if (debug) std::cout << "lex: tk_number " << nus << " " << val << "\n";
         }
         else
         if (isletter(*it))
         {
            std::string sym = "";
            sym += *it;
            it++;
            while (!end() && (isletter(*it) || isdigit(*it) || isextra(*it)))
            {
               sym += *it;
               it++;
            }
            token_text = sym;
            
            if (token_text == "defn")
            {
               tok = tk_defn;
            }
            else
            if (token_text == "let")
            {
               tok = tk_let;
            }
            else
            if (token_text == "fn")
            {
               if (debug) std::cout << "lex: tk_fn\n";
               tok = tk_fn;
            }
            else
            if (token_text == "if")
            {
               tok = tk_if;
            }
            else
            if (token_text == "println")
            {
               tok = tk_println;
            }
            else
            if (token_text == "not=")
            {
               tok = tk_notequal;
            }
            else
            if (token_text == ">=")
            {
               tok = tk_greatereq;
            }
            else
            if (token_text == "<=")
            {
               tok = tk_lesseq;
            }
            else
            if (token_text == "true")
            {
               tok = tk_true;
            }
            else
            if (token_text == "false")
            {
               tok = tk_false;
            }
            else
            if (token_text == "nil")
            {
               tok = tk_nil;
            }
            else
            if (token_text == "nil?")
            {
               tok = tk_builtin;
            }
            else
            if (token_text == "empty?")
            {
               tok = tk_builtin;
            }
            else
            {
               tok = tk_symbol;
               if (debug) std::cout << "lex: tk_symbol " << sym << "\n";
            }
         }
         else
         if (*it == '\"')
         {
            it++;  // skip "
            
            std::string tes = "";
            while (!end() && *it != '\"')
            {
               tes += *it;
               it++;
            }
            it++; // skip "
            
            tok        = tk_text;
            token_text = tes;
            if (debug) std::cout << "lex: tk_text " << tes << "\n";
         }
         else
         {
            tok = tk_eof;
            std::cout << "lex: tk_eof 2\n";
            exit(1);
         }
   }
}

