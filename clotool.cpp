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
   Parser_p parser = std::make_shared<Parser>();
   Element_p root = parser->parse("vb1.clj");
   if (root != nullptr)
   {
      root->show(0);
      
      Runner_p ru = std::make_shared<Runner>(root);
      Element_p res = ru->run();
      std::cout << "----- result ------\n";
      res->show(0);
   }
}

