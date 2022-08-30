#include "Allocator.hpp"
#include <cassert>
#include <cstring>

#ifndef ALIGN
#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) - 1))
#endif

namespace Meta {

Allocator::Allocator(size_t data_size, size_t page_size, size_t alignment) : m_PageList(nullptr), m_FreeList(nullptr) {
  Reset(data_size, page_size, alignment);
}

void Allocator::Reset(size_t data_size, size_t page_size, size_t alignment) {
  FreeAll();

  m_DataSize = data_size;
  m_PageSize = page_size;

  size_t minimal_size = (sizeof(BlockHeader) > m_DataSize) ? sizeof(BlockHeader) : m_DataSize;

#if defined(_DEBUG)
  assert(alignment > 0 && (alignment & (aligment-1)) == 0);
#endif
  m_BlockSize = ALIGN(minimal_size, alignment);

  m_AlignmentSize = m_BlockSize - minimal_size;

  m_BlocksPerPage = (m_PageSize - sizeof(PageHeader)) / m_BlockSize;
}

void* Allocator::Allocate() {
  if (!m_FreeList) {
    // allocate a new page
    PageHeader* pNewPage = reinterpret_cast<PageHeader*>(new uint8_t[m_PageSize]);
    ++m_Pages;
    m_Blocks += m_BlocksPerPage;
    m_FreeBlocks += m_BlocksPerPage;
#if defined(_DEBUG)
    FillFreePage(pNewPage);
#endif
    if (m_PageList) {
        pNewPage->pNext = m_PageList;
    }
    m_PageList = pNewPage;

    BlockHeader* pBlock = pNewPage->Blocks();
    // link each block in the page
    for (uint32_t i = 0; i < m_BlocksPerPage; i ++) {
      pBlock->pNext = NextBlock(pBlock);
      pBlock = NextBlock(pBlock);
    }
    pBlock->pNext = nullptr;

    m_FreeList = pNewPage->Blocks();
  }

  BlockHeader* freeBlock = m_FreeList;
  m_FreeList = m_FreeList->pNext;
  m_FreeBlocks--;

#if defined(_DEBUG)
  FillAllocatedBlock(freeBlock);
#endif

  return reinterpret_cast<void*>(freeBlock);
}

void Allocator::Free(void *p) {
  BlockHeader* block = reinterpret_cast<BlockHeader*>(p);

#if defined(_DEBUG)
  FillFreeBlock(block);
#endif

  block->pNext = m_FreeList;
  m_FreeList = block;
  ++m_FreeBlocks;
}

void Allocator::FreeAll() {
  PageHeader* pPage = m_PageList;
  while (pPage) {
    PageHeader* p = pPage;
    pPage = pPage->pNext;

    delete [] reinterpret_cast<uint8_t*>(p);
  }

  m_PageList = nullptr;
  m_FreeList = nullptr;

  m_Pages = 0;
  m_Blocks = 0;
  m_FreeBlocks = 0;
}

#if defined(_DEBUG)
void Allocator::FillFreePage(PageHeader* pPage) {
  pPage->pNext = nullptr;

  BlockHeader* pBlock = pPage->Blocks();
  for (uint32_t i = 0; i < m_BlocksPerPage; i ++) {
    FillFreeBlock(pBlock);
    pBlock = NextBlock(pBlock);
  }
}

void Allocator::FillFreeBlock(BlockHeader* pBlock) {
  std::memset(pBlock, PATTERN_FREE, m_BlockSize - m_AlignmentSize);

  std::memset(reinterpret_cast<uint8_t*>(pBlock) + m_BlockSize - m_AlignmentSize, PATTERN_ALIGN, m_AlignmentSize);
}

void My::Allocator::FillAllocatedBlock(BlockHeader *pBlock)
{
    std::memset(pBlock, PATTERN_ALLOC, m_BlockSize - m_AlignmentSize);

    std::memset(reinterpret_cast<uint8_t*>(pBlock) + m_BlockSize - m_AlignmentSize,
                PATTERN_ALIGN, m_AlignmentSize);
}
#endif

BlockHeader* Allocator::NextBlock(BlockHeader *pBlock) {
  return reinterpret_cast<BlockHeader*>(reinterpret_cast<uint8_t*>(pBlock) + m_BlockSize);
}

}