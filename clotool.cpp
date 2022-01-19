#include <iostream>
#include <getopt.h>
#include <string>
#include <type_traits>

#include <unistd.h>
#include <stdlib.h>

#include "easylogging++.h"

#include "parser.h"
#include "runner.h"


INITIALIZE_EASYLOGGINGPP

enum class Channels
{
   MAIN     = 1 << 0,
   TEXTFILE = 1 << 1,
   LEX      = 1 << 2,
   PARSER   = 1 << 3,
   RUNNER   = 1 << 4,
   RECURSE  = 1 << 5,
   CAPTURE  = 1 << 6,
   DEBUGGER = 1 << 7
};

template <typename E>
constexpr auto to_underlying(E e) noexcept
{
    return static_cast<std::underlying_type_t<E>>(e);
}

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

   // levels
   //    1 Global
   //    2 Trace
   //    4 Debug
   //    8 Fatal
   //   16 Error
   //   32 Warning
   //   64 Verbose
   //  128 Info
   // 1010 Unknown
   void enable(int levels)
   {
      el::Configurations conf;
      conf.setGlobally(el::ConfigurationType::Format, "%level %logger %msg");

      // Global
      if ((levels & to_underlying(el::Level::Global)) != 0)
      {
         conf.set(el::Level::Global, el::ConfigurationType::Enabled, "true");
      }
      else
      {
         conf.set(el::Level::Global, el::ConfigurationType::Enabled, "false");
      }

      // Trace
      if ((levels & to_underlying(el::Level::Trace)) != 0)
      {
         conf.set(el::Level::Trace, el::ConfigurationType::Enabled, "true");
      }
      else
      {
         conf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false");
      }

      // Debug
      if ((levels & to_underlying(el::Level::Debug)) != 0)
      {
         conf.set(el::Level::Debug, el::ConfigurationType::Enabled, "true");
      }
      else
      {
         conf.set(el::Level::Debug, el::ConfigurationType::Enabled, "false");
      }

      // Fatal
      if ((levels & to_underlying(el::Level::Fatal)) != 0)
      {
         conf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true");
      }
      else
      {
         conf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "false");
      }

      // Error
      if ((levels & to_underlying(el::Level::Error)) != 0)
      {
         conf.set(el::Level::Error, el::ConfigurationType::Enabled, "true");
      }
      else
      {
         conf.set(el::Level::Error, el::ConfigurationType::Enabled, "false");
      }

      // Warning
      if ((levels & to_underlying(el::Level::Warning)) != 0)
      {
         conf.set(el::Level::Warning, el::ConfigurationType::Enabled, "true");
      }
      else
      {
         conf.set(el::Level::Warning, el::ConfigurationType::Enabled, "false");
      }

      // Verbose
      if ((levels & to_underlying(el::Level::Verbose)) != 0)
      {
         conf.set(el::Level::Verbose, el::ConfigurationType::Enabled, "true");
      }
      else
      {
         conf.set(el::Level::Verbose, el::ConfigurationType::Enabled, "false");
      }

      // Info
      if ((levels & to_underlying(el::Level::Info)) != 0)
      {
         conf.set(el::Level::Info, el::ConfigurationType::Enabled, "true");
      }
      else
      {
         conf.set(el::Level::Info, el::ConfigurationType::Enabled, "false");
      }

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



//bool debug   = false;
bool trans    = false;
bool showclj  = false;
bool debugger = false;

// options
//
//   f file
//   d debug level
//   c log channels
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
   logmain.disable();
   CLOG(DEBUG, "main") << "start coloru";

   Logchannel logtextfile("textfile");
   logtextfile.disable();

   Logchannel loglex("lex");
   loglex.disable();

   Logchannel logparser("parser");
   logparser.disable();

   Logchannel logrunner("runner");
   logrunner.disable();

   Logchannel logrecurse("recurse");
   logrecurse.disable();

   Logchannel logcapture("capture");
   logcapture.disable();

   Logchannel logdebugger("debugger");
   logdebugger.enable();


   char *fname;
   int   dlevel;
   int   channels;
   int   opt;
   while ((opt = getopt(argc, argv, "eltc:d:f:")) != -1)
   {
      switch (opt)
      {
         case 'f':
            fname = optarg;
            break;

         case 'c':
            //debug = true;
            try
            {
               channels = std::stoi(optarg);
            }
            catch (std::invalid_argument &e)
            {
               std::cout << "use integer as argument for -c\n";
               exit(1);
            }
            break;

         case 'd':
            //debug = false;
            try
            {
               dlevel = std::stoi(optarg);
            }
            catch (std::invalid_argument &e)
            {
               std::cout << "use integer as argument for -d\n";
               exit(1);
            }
            break;

         case 't':
            trans = true;
            break;

         case 'l':
            showclj = true;
            break;

         case 'e':
            debugger = true;
            break;
      }
   }

   // enable all the loggers if necesary
   if ((channels & to_underlying(Channels::MAIN)) != 0)
   {
      logmain.enable(dlevel);
   }
   if ((channels & to_underlying(Channels::TEXTFILE)) != 0)
   {
      logtextfile.enable(dlevel);
   }
   if ((channels & to_underlying(Channels::LEX)) != 0)
   {
      loglex.enable(dlevel);
   }
   if ((channels & to_underlying(Channels::PARSER)) != 0)
   {
      logparser.enable(dlevel);
   }
   if ((channels & to_underlying(Channels::RUNNER)) != 0)
   {
      logrunner.enable(dlevel);
   }
   if ((channels & to_underlying(Channels::RECURSE)) != 0)
   {
      logrecurse.enable(dlevel);
   }
   if ((channels & to_underlying(Channels::CAPTURE)) != 0)
   {
      logcapture.enable(dlevel);
   }

   CLOG(DEBUG, "main") << "start";


   Parser_p parser = std::make_shared<Parser>();
   Element_p root = parser->parse(fname);
   if (root != nullptr)
   {
      if (trans)
      {
         CLOG(DEBUG, "main") << "%%%%%%%%%%%%%%%%%%% makeTail1 %%%%%%%%%%%%%%%%%%%";
         bool r = root->makeTail();
         CLOG(DEBUG, "main") << "makeTail1 result " << r;

         if (r)
         {
            CLOG(DEBUG, "main") << "%%%%%%%%%%%%%%%%%%% makeTail2 %%%%%%%%%%%%%%%%%%%";
            r = root->makeTail();
            CLOG(DEBUG, "main") << "makeTail2 result " << r;
         }
      }

      if (!showclj)
      {
         if (!debugger)
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
         else
         {
            Runner_p ru = std::make_shared<Runner>(root);
            Element_p res = ru->debugger();
            std::cout << "Result:\n";
            if (res != nullptr)
            {
               res->show(1, "debugger");
            }
            else
            {
               std::cout << "nullptr\n";
            }
         }
      }
      CLOG(DEBUG, "main") << "---- root ----";
      root->show(0, "main");
   }
}


