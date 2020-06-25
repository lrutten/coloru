#include <iostream>

#include "parser.h"
#include "runner.h"

bool debug = false;

int main(int argc, char **argv)
{
   if (debug) std::cout << "start\n";
   
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
   Element_p root = parser->parse(argv[1]);
   if (root != nullptr)
   {
      if (debug) root->show(0);
      
      Runner_p ru = std::make_shared<Runner>(root);
      Element_p res = ru->run();
      std::cout << "Result:\n";
      res->show(1);
   }
}

