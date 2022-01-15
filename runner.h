#ifndef RUNNER_H
#define RUNNER_H

#include <deque>
#include <map>

#include "parser.h"


enum frame_t
{
   fr_undefined,
   fr_main,
   fr_defn,
   fr_lambda,
   fr_capture,
   fr_let
};

// exception klassen

class RunError
{
};


class Frame : public Element
{
public:
   Frame();
   Frame(std::string inf);
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
   int getFrType()
   {
      return frtype;
   }
   void setFrType(frame_t tp)
   {
      frtype = tp;
   }
   std::string frametp_as_s();
   std::string getInfo()
   {
      return info;
   }
   
private:
   int         nr;
   std::map<std::string, Element_p> bindings;
   frame_t     frtype;
   std::string info;
};

using Frame_p = std::shared_ptr<Frame>;

class Context
{
public:
   Context();
   ~Context();
   void push(frame_t frtp);
   void push(Frame_p fr);
   void pop();
   void add_binding(std::string nm, Element_p);
   Element_p search(std::string nm, bool shortsrch = false);
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

