#include <iostream>

#include "parser.h"
#include "runner.h"

bool debug = false;

int main(int argc, char **argv)
{
   if (debug) std::cout << "start\n";
   
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

