#ifndef TEXTFILE_H
#define TEXTFILE_H

#include <string>
#include <vector>
#include <tuple>
#include <memory>

class Textfile : public std::enable_shared_from_this<Textfile>
{
public:
   explicit Textfile(const std::string &fn);
   ~Textfile();
   void readfile();
   void show();
   std::string remove_comment(std::string line);

   template <typename T>
   class const_iterator
   {
   public:
      using size_type         = std::size_t;
      using self_type         = const_iterator;
      using value_type        = T;
      using reference         = T&;
      using pointer           = std::tuple<size_type, size_type>;
      using difference_type   = int;
      using iterator_category = std::forward_iterator_tag;
      
      const_iterator(const std::shared_ptr<const Textfile> tf, pointer ptr) : tf_(tf), ptr_(ptr)
      {
      }

      self_type operator++()
      {
         self_type i = *this;
         size_type jf;
         size_type js;
         std::tie(jf, js) = ptr_;
         if (js < tf_->lines[jf].size())
         {
            js++;
            if (js == tf_->lines[jf].size())
            {
               jf++;
               js = 0;
            }
         }
         //std::cout << "jf js " << jf << " " << js << "\n";
         
         ptr_ = std::tuple<size_type, size_type>{jf, js};
         return i;
      }

      self_type operator++(int junk)
      {
         size_type jf;
         size_type js;
         std::tie(jf, js) = ptr_;
         if (js < tf_->lines[jf].size())
         {
            js++;
            if (js == tf_->lines[jf].size())
            {
               jf++;
               js = 0;
            }
         }
         //std::cout << "jf js " << jf << " " << js << "\n";
         
         ptr_ = std::tuple<size_type, size_type>{jf, js};
         return *this;
      }

      const value_type operator*()
      {
         size_type jf;
         size_type js;
         std::tie(jf, js) = ptr_;
         return tf_->lines[jf][js];
      }
      
      const pointer operator->()
      {
         return ptr_;
      }

      bool operator==(const self_type& rhs)
      { 
         size_type jf1;
         size_type js1;
         std::tie(jf1, js1) = ptr_;
         size_type jf2;
         size_type js2;
         std::tie(jf2, js2) = rhs.ptr_;

         return jf1 == jf2 && js1 == js2;
         //return ptr_ == rhs.ptr_;
      }

      bool operator!=(const self_type& rhs)
      {
         return ptr_ != rhs.ptr_;
      }

   private:
      const std::shared_ptr<const Textfile> tf_;
      pointer                       ptr_;
   };

   
   const_iterator<char> begin() const
   {
      return const_iterator<char>(shared_from_this(), const_iterator<char>::pointer{0, 0});
   }

   const_iterator<char> end() const
   {
      return const_iterator<char>(shared_from_this(), const_iterator<char>::pointer{lines.size(), 0});
   }

   
private:
   std::string              filename;
   std::vector<std::string> lines;
};

using Textfile_p = std::shared_ptr<Textfile>;

#endif

