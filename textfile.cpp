#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include "textfile.h"

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

   std::cout << "readfile " << filename << "\n";

   fin.open(filename);
   std::cout << "na open\n";
   
   if(!fin.is_open())
   {
      std::cout << "Error at reading file!\n";
      //exit(1);
   }
   else
   {
      std::cout << "open ok\n";
   
      getline(fin, line);
      while(!fin.eof())
      {
         std::cout << "line read " << line << "\n";
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
