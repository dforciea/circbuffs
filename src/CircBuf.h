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

      class Path
      {
      public:
         enum PathType { DIR, FILE };

         Path() : m_name(""), m_parent(0), m_type(DIR), m_file_idx(0), m_children() { }
         Path(const std::string& name, PathType type, Path* parent, std::size_t file_idx = 0) : 
            m_name(name), m_parent(parent), m_type(type), m_file_idx(file_idx), m_children() { }

         inline Path& add_child(const std::string& name, PathType type, std::size_t file_idx = 0);
         inline Path& find_child(const std::string& name);
         inline Path& move_path(const std::string& from, const std::string& to);

         const std::string& get_name() const { return m_name; }
         
      private:
         inline void find_child(const std::string& name, std::string::size_type cur_offset, Path*& cur_path);
         std::string m_name;
         Path* m_parent;
         PathType m_type;
         std::size_t m_file_idx;
         std::map<std::string, Path> m_children;
      };
   private:
     
      struct block_desc_t
      {
         std::size_t file_idx;
         std::size_t next_block;
      };

      struct file_desc_t
      {
         Path path;
         std::size_t first_block_idx;
         std::size_t cur_seek_block_idx; 
         std::size_t size;
      };

      std::size_t m_size;
      std::size_t m_block_size;

      void* m_buffer;
      std::size_t m_cur_pos;

      std::vector<block_desc_t> m_block_descs;
      std::vector<file_desc_t> m_file_descs;

      Path m_root;
   };

   CircBuf::Path& 
   CircBuf::Path::add_child(const std::string& name, PathType type, std::size_t file_idx)
   {
      std::map<std::string, Path>::iterator itr = m_children.find(name);
      if(itr == m_children.end())
      {
         m_children[name] = Path(name, type, this, file_idx);
         return m_children[name];
      }
      else
      {
         throw std::invalid_argument("Filename already exists");
      }
   }

   CircBuf::Path&
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
   
   CircBuf::Path&
   CircBuf::Path::move_path(const std::string& from, const std::string& to)
   {
      Path& from_path = find_child(from);
      if(from_path.m_parent == 0)
      {
         throw std::invalid_argument("Cannot move the root path");
      }
      Path& to_path = find_child(to);

      std::map<std::string, Path>::iterator itr = to_path.m_children.find(from_path.get_name());
      if(itr != to_path.m_children.end())
      {
         throw std::invalid_argument("The destination already contains the name");
      }
      to_path.m_children[from_path.get_name()] = from_path;
      from_path.m_parent->m_children.erase(from_path.get_name());
   }

}

#endif
