#include <iostream>
#include <getopt.h>

#include "easylogging++.h"

#include "parser.h"
#include "runner.h"

INITIALIZE_EASYLOGGINGPP


class Logchannel
{
public:
   Logchannel(std::string nm) : name(nm)
   {
      logger = el::Loggers::getLogger(nm);
   }

   void enable()
   {
      el::Configurations conf;
      conf.setGlobally(el::ConfigurationType::Format, "%level %logger %msg");
      conf.setGlobally(el::ConfigurationType::Enabled, "true");
      el::Loggers::reconfigureLogger(name, conf);
   }

   void disable()
   {
      el::Configurations conf;
      conf.setGlobally(el::ConfigurationType::Enabled, "false");
      el::Loggers::reconfigureLogger(name, conf);
   }

private:
   std::string  name;
   el::Logger *logger;
};




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
   //START_EASYLOGGINGPP(argc, argv);

   // ---- default logger -----
   el::Configurations defaultConf;
   defaultConf.setToDefault();

   // Values are always std::string
   defaultConf.set(el::Level::Info,
            el::ConfigurationType::Format, "%datetime %level %msg");
   // default logger uses default configurations
   el::Loggers::reconfigureLogger("default", defaultConf);
   //LOG(INFO) << "Log using default file";



   Logchannel logmain("main");
   logmain.enable();
   CLOG(DEBUG, "main") << "start coloru";

   Logchannel loglex("lex");
   loglex.disable();

   Logchannel logparser("parser");
   logparser.enable();

   Logchannel logrunner("runner");
   logrunner.enable();

   Logchannel logtextfile("textfile");
   logtextfile.enable();


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

   CLOG(DEBUG, "main") << "start";


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
            res->show(1, "main");
         }
         else
         {
            std::cout << "nullptr\n";
         }
      }
      if (debug2) root->show(0, "main");
   }
}

