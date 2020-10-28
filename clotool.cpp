#include <iostream>
#include <getopt.h>

#include "parser.h"
#include "runner.h"

bool debug   = false;
bool debug2  = false;
bool trans   = false;
bool showclj = false;

// options
//
//   f file
//   d debug
//   t transform to tail recursion
//   l show transformed source code
//
int main(int argc, char **argv)
{
   char *fname;
   int opt;
   while ((opt = getopt(argc, argv, "ltdf:")) != -1)
   {
      switch (opt)
      {
         case 'f':
            fname = optarg;
            break;

         case 'd':
            debug = true;
            break;

         case 't':
            trans = true;
            break;

         case 'l':
            showclj = true;
            break;
      }
   }

   if (debug) std::cout << "start\n";

   
   Parser_p parser = std::make_shared<Parser>();
   Element_p root = parser->parse(fname);
   if (root != nullptr)
   {
      if (trans)
      {
         root->makeTail();
      }
      
      if (!showclj)
      {
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
      }
      if (debug2) root->show(0);
   }
}

