#include <cstdio>
#include <vector>
#include <stdint.h>

namespace circbuffs
{
   class CircBuf
   {
   public:
      CircBuf(std::size_t size, std::size_t block_size = 4096);
      ~CircBuf();
   private:
      struct block_desc_t
      {
         std::size_t file_id;
         std::size_t next_block;
         std::size_t size;
      };

      size_t m_size;
      size_t m_block_size;

      void* m_buffer;
      size_t m_cur_pos;

      std::vector<block_desc_t> m_block_descs;
   };
}

