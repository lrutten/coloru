#ifndef RUNNER_H
#define RUNNER_H

#include <deque>
#include <map>

#include "parser.h"

// exception klassen

class RunError
{
};


class Frame : public Element
{
public:
   Frame();
   ~Frame();
   void add_binding(std::string nm, Element_p el);
   Element_p search(std::string nm);
   bool exists(std::string nm);
   void show(int d, const std::string &chan);

   void format(int d) override
   {
   }
   std::shared_ptr<Element> evaluate(std::shared_ptr<Context> cx, int d) override
   {
      return nullptr;
   }
   type_t getType() override
   {
      return tp_undefined;
   }
   void resetTreetype() override
   {
   }
   void determTreetype(std::shared_ptr<Main> main, std::shared_ptr<Defn> defn) override
   {
   }
   int getNr()
   {
      return nr;
   }
   void setNr(int n)
   {
      nr = n;
   }

private:
   int nr;
   std::map<std::string, Element_p> bindings;
};

using Frame_p = std::shared_ptr<Frame>;

class Context
{
public:
   Context();
   ~Context();
   void push();
   void push(Frame_p fr);
   void pop();
   void add_binding(std::string nm, Element_p);
   Element_p search(std::string nm);
   bool exists(std::string nm);
   void show(int d, const std::string &chan);

private:
   std::deque<Frame_p> frames;
};

using Context_p = std::shared_ptr<Context>;


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

