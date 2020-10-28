#include <iostream>
#include <getopt.h>

#include "parser.h"
#include "runner.h"

bool debug  = false;
bool debug2 = false;
bool trans  = false;

int main(int argc, char **argv)
{
   char *fname;
   int opt;
   while ((opt = getopt(argc, argv, "tdf:")) != -1)
   {
      switch (opt)
      {
         case 'd':
            debug = true;
            break;

         case 't':
            trans = true;
            break;

         case 'f':
            fname = optarg;
            break;
      }
   }

   if (debug) std::cout << "start\n";

   
   Parser_p parser = std::make_shared<Parser>();
   Element_p root = parser->parse(fname);
   //Element_p root = parser->parse(argv[1]);
   //Element_p root = parser->parse("vb3.clj");
   if (root != nullptr)
   {
      /*
         moved
         
      root->resetTreetype();
      root->determTreetype(nullptr);

      if (debug) std::cout << "=========transform=============\n";
      root->transformTree(0);

      if (debug) std::cout << "=========show=1================\n";
      if (debug) root->show(0);
      
      if (debug) std::cout << "=========format================\n";
      if (debug || trans) root->format(0);

      if (debug) std::cout << "=========show 2================\n";
      root->resetTreetype();
      root->determTreetype(nullptr);
      if (debug) root->show(0);
      if (debug) std::cout << "===============================\n";
      */

      root->makeTail();
      
      Runner_p ru = std::make_shared<Runner>(root);
      Element_p res = ru->run();
      std::cout << "Result:\n";
      if (res != nullptr)
      {
         res->show(1);
      }
      else
      {
         std::cout << "nullptr\n";
      }
      if (debug2) root->show(0);
   }
}

