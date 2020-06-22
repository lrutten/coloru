#ifndef LEX_H
#define LEX_H

#include "textfile.h"

enum token_t
{
   tk_bropen,
   tk_brclose,
   tk_rbopen,
   tk_rbclose,
   tk_plus,
   tk_minus,
   tk_mul,
   tk_div,
   tk_ampersand,
   tk_apostrophe,
   tk_equal,
   tk_notequal,
   tk_less,
   tk_lesseq,
   tk_greater,
   tk_greatereq,
   tk_keyword,   // name with ':' prefix
   tk_symbol,    // name
   tk_defn,
   tk_let,
   tk_fn,
   tk_if,
   tk_println,
   tk_number,
   tk_true,
   tk_false,
   tk_eof
};


using number_t   = long long;
using iterator_t = Textfile::const_iterator<char>;

class Lex
{
public:
   explicit Lex(const std::string &filename);
   ~Lex();
   void     next();
   token_t  token();
   number_t value();
   int      charnr();
   std::string   text()
   {
      return token_text;
   }

protected:
   bool     end();
   
private:
   Textfile_p   file;
   iterator_t   it;
   token_t      tok;
   number_t     val;
   std::string  token_text;
};

using Lex_p = std::shared_ptr<Lex>;

#endif

