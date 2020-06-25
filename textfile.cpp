#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include "textfile.h"

extern bool debug;

Textfile::Textfile(const std::string &fn) : filename(fn)
{
   readfile();
}

Textfile::~Textfile()
{
}


void Textfile::readfile()
{
   std::string   line;
   std::ifstream fin;

   if (debug) std::cout << "readfile " << filename << "\n";

   fin.open(filename);
   if (debug) std::cout << "na open\n";
   
   if(!fin.is_open())
   {
      std::cout << "Error at reading file!\n";
      //exit(1);
   }
   else
   {
      if (debug) std::cout << "open ok\n";
   
      getline(fin, line);
      while(!fin.eof())
      {
         if (debug) std::cout << "line read " << line << "\n";
         lines.push_back(line + "\n");
   
         getline(fin, line);
      }
      std::cout << "Read " << lines.size() << "\n";
   }
}

void Textfile::show()
{
   std::cout << "---- lines ----\n";
   std::cout << "   #" << lines.size() << "\n";
   for (std::string line: lines)
   {
      std::cout << "line " << line << "\n";
   }
   std::cout << "---- ------ ----\n";
}
