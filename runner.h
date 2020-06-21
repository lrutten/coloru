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
   explicit Runner(Element_p rt);
   virtual ~Runner()
   {
   }
   Element_p run();

private:
   Element_p root;
};

using Runner_p = std::shared_ptr<Runner>;

#endif

