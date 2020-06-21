#include <iostream>

#include "parser.h"
#include "runner.h"

int main()
{
   std::cout << "start\n";
   
   /*
   Textfile *fi = new Textfile("vb1.clj");
   fi->readfile();
   fi->show();
   
   for (char c: *fi)
   {
      std::cout << c;
   }

   
   for (auto it = fi->begin(); it != fi->end(); it++)
   {
      char c = *it;
      std::cout << c;
   }      
   delete fi;
    */
   
   /*
   Lex *lex = new Lex("vb1.clj");
   while (lex->token() != tk_eof)
   {
      lex->next();
   }
   delete lex;
    */
   Parser *parser = new Parser();
   Element *root = parser->parse("vb1.clj");
   if (root != nullptr)
   {
      root->show(0);
      
      Runner *ru = new Runner(root);
      Element *res = ru->run();
      std::cout << "----- result ------\n";
      res->show(0);
      delete res;
      delete ru;
      
   }
   delete root;
   delete parser;
}

