#ifndef CIRCBUF_H
#define CIRCBUF_H

#include <string>
#include <map>
#include <cstdio>
#include <vector>
#include <stdexcept>
#include <stdint.h>

namespace circbuffs
{
   class CircBuf
   {
   public:
      CircBuf(std::size_t size, std::size_t block_size = 4096);
      ~CircBuf();
   private:
      class Path
      {
      public:
         Path() : m_name("") { }
         Path(const std::string& name) : m_name(name) { }

         inline bool add_child(const Path& child);
         inline const Path& find_child(const std::string& name);

         const std::string& get_name() const { return m_name; }
         
      private:
         inline void find_child(const std::string& name, std::string::size_type cur_offset, Path*& cur_path);
         std::string m_name;
         std::map<std::string, Path> m_children;
      };
     
      struct block_desc_t
      {
         std::size_t file_id;
         std::size_t next_block;
         std::size_t size;
      };

      struct file_desc_t
      {
         std::string path;
      };

      size_t m_size;
      size_t m_block_size;

      void* m_buffer;
      size_t m_cur_pos;

      std::vector<block_desc_t> m_block_descs;
      std::vector<file_desc_t> m_file_descs;

      Path m_root;
   };

   bool
   CircBuf::Path::add_child(const CircBuf::Path& child)
   {
      std::map<std::string, Path>::iterator itr = m_children.find(child.get_name());
      if(itr == m_children.end())
      {
         m_children[child.get_name()] = child;
         return true;
      }
      else
      {
         return false;
      }
   }

   const CircBuf::Path&
   CircBuf::Path::find_child(const std::string& name)
   {
      Path* cur_path = this;
      find_child(name, 0, cur_path);
      return *cur_path;
   }

   void
   CircBuf::Path::find_child(const std::string& name, std::string::size_type cur_offset, CircBuf::Path*& cur_path)
   {
      std::string cur_element;

      std::string::size_type n = name.find('/', cur_offset);
      if(n != std::string::npos)
      {
         cur_element = name.substr(cur_offset, n - cur_offset);
         cur_offset = n;
      }
      else
      {
         cur_element = name.substr(cur_offset, name.size() - cur_offset);
         cur_offset = name.size();
      }

      if(cur_element.empty())
      {
         if(cur_offset == name.size())
         {
            throw std::invalid_argument("Path not found!");
         }
         else
         {
            return find_child(name, cur_offset, cur_path);
         }
      }

      std::map<std::string, Path>::iterator itr = cur_path->m_children.find(cur_element);
      if(itr != cur_path->m_children.end())
      {
         cur_path = &itr->second;
         find_child(name, cur_offset, cur_path);
      }
      else
      {
         throw std::invalid_argument("Path not found!");
      }
   }
}

#endif
