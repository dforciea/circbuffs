#include <CircBuf.h>

#include <cstring>

#include <errno.h>
#include <sys/mman.h>

namespace circbuffs
{
   CircBuf::CircBuf(uint64_t size, uint64_t block_size) :
      m_size(size),
      m_block_size(block_size),
      m_buffer(0),
      m_cur_pos(0),
      m_block_descs(0),
      m_root()
   {
      if(size % block_size != 0)
      {
         throw std::invalid_argument("size must be a multiple of block_size!");
      }

      m_block_descs.reserve(size / block_size);
      m_block_descs.resize(size / block_size);

      m_buffer = mmap(NULL, m_size, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS,
                      -1, 0);
      if(m_buffer == MAP_FAILED)
      {
        throw std::invalid_argument(strerror(errno)); 
      }
   }

   CircBuf::~CircBuf()
   {
      munmap(m_buffer, m_size);
   }
}
