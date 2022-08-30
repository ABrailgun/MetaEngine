#include <cstddef>
#include <cstdint>

namespace Meta {

struct BlockHeader {
  BlockHeader* pNext;
};

struct PageHeader {
  PageHeader* pNext;
  BlockHeader* Blocks() {
    return reinterpret_cast<BlockHeader*>(this + 1);
  }
};

class Allocator {
 public:
  // debug patterns
  static const uint8_t PATTERN_ALIGN = 0xFC;
  static const uint8_t PATTERN_ALLOC = 0xFD;
  static const uint8_t PATTERN_FREE = 0xFE;

  Allocator(size_t data_size, size_t page_size, size_t alignment);

  ~Allocator();

  // reset the allocator to a new configuration
  void Reset(size_t data_size, size_t page_size, size_t alignment);

  // alloc and free blocks
  void* Allocate();
  void Free(void* p);
  void FreeAll();

 private:
#if defined(_DEBUG)
  // fill a free page with debug patterns
  void FillFreePage(PageHeader* pPage);

  // fill a free block with debug patterns
  void FillFreeBlock(BlockHeader* pBlock);

  // fill an allocated block with debug patterns
  void FillAllocatedBlock(BlockHeader* pBlock);
#endif

  // gets the next block
  BlockHeader* NextBlock(BlockHeader* pBlock);

  // the page list
  PageHeader* m_PageList;

  // the free block list
  BlockHeader* m_FreeList;

  size_t m_DataSize;
  size_t m_PageSize;
  size_t m_AlignmentSize;
  size_t m_BlockSize;
  uint32_t m_BlocksPerPage;

  uint32_t m_Pages;
  uint32_t m_Blocks;
  uint32_t m_FreeBlocks;

  Allocator(const Allocator& clone) = delete;
  Allocator &operator=(const Allocator& rhs) = delete;





};

}