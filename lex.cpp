#include <iostream>
#include <sstream>
#include <string.h>

#include "easylogging++.h"
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
   CLOG(DEBUG, "lex") << "start";
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
   CLOG(DEBUG, "lex") << "teken " << *it;

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
            CLOG(DEBUG, "lex") << "tk_number " << nus << " " << val;
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
               CLOG(DEBUG, "lex") << "defn";
            }
            else
            if (token_text == "let")
            {
               tok = tk_let;
               CLOG(DEBUG, "lex") << "let";
            }
            else
            if (token_text == "fn")
            {
               tok = tk_fn;
               CLOG(DEBUG, "lex") << "tk_fn";
            }
            else
            if (token_text == "if")
            {
               tok = tk_if;
               CLOG(DEBUG, "lex") << "if";
            }
            else
            if (token_text == "println")
            {
               tok = tk_println;
               CLOG(DEBUG, "lex") << "println";
            }
            else
            if (token_text == "print")
            {
               tok = tk_print;
               CLOG(DEBUG, "lex") << "print";
            }
            else
            if (token_text == "not=")
            {
               tok = tk_notequal;
               CLOG(DEBUG, "lex") << "not=";
            }
            else
            if (token_text == ">=")
            {
               tok = tk_greatereq;
               CLOG(DEBUG, "lex") << ">=";
            }
            else
            if (token_text == "<=")
            {
               tok = tk_lesseq;
               CLOG(DEBUG, "lex") << "<=";
            }
            else
            if (token_text == "true")
            {
               tok = tk_true;
               CLOG(DEBUG, "lex") << "true";
            }
            else
            if (token_text == "false")
            {
               tok = tk_false;
               CLOG(DEBUG, "lex") << "false";
            }
            else
            if (token_text == "nil")
            {
               tok = tk_nil;
               CLOG(DEBUG, "lex") << "nil";
            }
            else
            if (token_text == "nil?")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "nil?";
            }
            else
            if (token_text == "empty?")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "empty?";
            }
            else
            if (token_text == "context!")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "context!";
            }
            else
            if (token_text == "cons")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "cons";
            }
            else
            if (token_text == "seq?")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "seq?";
            }
            else
            if (token_text == "list")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "list";
            }
            else
            if (token_text == "and")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "and";
            }
            else
            if (token_text == "or")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "or";
            }
            else
            if (token_text == "not")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "not";
            }
            else
            if (token_text == "mod")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "mod";
            }
            else
            if (token_text == "nth")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "nth";
            }
            else
            if (token_text == "size")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "nth";
            }
            else
            if (token_text == "first")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "nth";
            }
            else
            if (token_text == "rest")
            {
               tok = tk_builtin;
               CLOG(DEBUG, "lex") << "nth";
            }
            else
            {
               tok = tk_symbol;
               CLOG(DEBUG, "lex") << "tk_symbol " << sym;
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
            CLOG(DEBUG, "lex") << "tk_text " << tes;
         }
         else
         {
            tok = tk_eof;
            std::cout << "lex: tk_eof 2\n";
            exit(1);
         }
   }
}

