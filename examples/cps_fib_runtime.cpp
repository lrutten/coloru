#include <iostream>
#include <functional>

int fib(int n)
{
  if (n <= 2)
    return 1;
  else
    return fib(n - 1)
           + fib(n - 2);
}

// Runtime CPS

void fib_cps(int n, std::function<void(int)> k)
{
   if (n <= 2)
   {
      k(1);
   }
   else
   {
      fib_cps(n - 1, [n,&k](int r1)
      {
         fib_cps(n - 2, [r1,&k](int r2)
         {
            k(r1 + r2);
         });
      });
   }
}

int main()
{
   std::cout << fib(1) << std::endl;
   fib_cps(1, [](int r){ std::cout << "1: " << r << std::endl; });

   std::cout << fib(2) << std::endl;
   fib_cps(2, [](int r){ std::cout << r << std::endl; });

   std::cout << fib(3) << std::endl;
   fib_cps(3, [](int r){ std::cout << r << std::endl; });

   std::cout << fib(4) << std::endl;
   fib_cps(4, [](int r){ std::cout << r << std::endl; });

   std::cout << fib(5) << std::endl;
   fib_cps(5, [](int r){ std::cout << r << std::endl; });

   std::cout << fib(6) << std::endl;
   fib_cps(6, [](int r){ std::cout << r << std::endl; });

   std::cout << fib(7) << std::endl;
   fib_cps(7, [](int r){ std::cout << r << std::endl; });
}

