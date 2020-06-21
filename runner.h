#ifndef RUNNER_H
#define RUNNER_H

//#include <vector>
//#include <deque>

#include "parser.h"

// exception klassen

class RunError
{
};

class Context
{
};

class Runner
{
public:
   Runner(Element *rt);
   virtual ~Runner()
   {
   }
   Element *run();

private:
   Element *root;
};

#endif

