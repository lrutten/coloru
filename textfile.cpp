#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include "easylogging++.h"
#include "textfile.h"

extern bool debug;

Textfile::Textfile(const std::string &fn) : filename(fn)
{
   readfile();
}

Textfile::~Textfile()
{
}

std::string Textfile::remove_comment(std::string line)
{
   auto pos = line.find_first_of(';');
   if (pos == std::string::npos)
   {
      return line;
   }
   else
   {
      // skip ; and the following characters
      return line.substr(0, pos);
   }
}

void Textfile::readfile()
{
   std::string   line;
   std::ifstream fin;

   CLOG(DEBUG, "textfile") << "readfile " << filename;

   fin.open(filename);
   CLOG(DEBUG, "textfile") << "na open";

   if(!fin.is_open())
   {
      std::cout << "Error at reading file!\n";
      //exit(1);
   }
   else
   {
      CLOG(DEBUG, "textfile") << "open ok";

      getline(fin, line);
      while(!fin.eof())
      {
         CLOG(DEBUG, "textfile") << "line read " << line;
         lines.push_back(remove_comment(line) + "\n");

         getline(fin, line);
      }
      CLOG(DEBUG, "textfile") << "Read " << lines.size();
   }
}

void Textfile::show()
{
   CLOG(DEBUG, "textfile") << "---- lines ----";
   CLOG(DEBUG, "textfile") << "   #" << lines.size();
   for (std::string line: lines)
   {
      CLOG(DEBUG, "textfile") << "line " << line;
   }
   CLOG(DEBUG, "textfile") << "---- ------ ----";
}
