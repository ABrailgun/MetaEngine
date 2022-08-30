#include "MemoryManager.hpp"
#include <malloc.h>

namespace Meta {

static const uint32_t kBlockSizes[] = {
  // 4-increments
  4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
  52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96,

  // 32-increments
  128, 160, 192, 224, 256, 288, 320, 352, 384,
  416, 448, 480, 512, 544, 576, 608, 640,

  // 64-increments
  704, 768, 832, 896, 960, 1024
};

static const uint32_t kPageSize = 8192;
static const uint32_t kAlignment = 4;

static const uint32_t kNumBlockSizes = sizeof(kBlockSizes) / sizeof(kBlockSizes[0]);

static const uint32_t kMaxBlockSize = kBlockSizes[kNumBlockSizes - 1];

int MemoryManager::Initialize() {
  // one-time initialization
  static bool initialized = false;
  if (!initialized) {
    // initialize block size lookup table
    m_BlockSizeLookup = new size_t[kMaxBlockSize + 1];
    size_t j = 0;
    for (size_t i = 0; i <= kMaxBlockSize; i ++) {
      if (i > kBlockSizes[j]) {
          j ++;
      }
      m_BlockSizeLookup[i] = j;
    }

    // initialize the allocators
    m_Allocators = new Allocator[kNumBlockSizes];
    for (size_t i = 0; i < kNumBlockSizes; i ++) {
      m_Allocators[i].Reset(kBlockSizes[i], kPageSize, kAlignment);
    }

    initialized = true;
  }

  return InitialStatus::Success;
}

void MemoryManager::Finalize() {
  delete []m_Allocators;
  delete []m_BlockSizeLookup;
}

void MemoryManager::Tick() {
}

Allocator* MemoryManager::LookUpAllocator(size_t size) {
  if (size <= kMaxBlockSize) {
    return m_Allocators + m_BlockSizeLookup[size];
  }
  return nullptr;
}

void* MemoryManager::Allocate(size_t size) {
  Allocator* allocator = LookUpAllocator(size);
  if (allocator) {
    return allocator->Allocate();
  }
  return malloc(size);
}

void MemoryManager::Free(void *p, size_t size) {
  Allocator* allocator = LookUpAllocator(size);
  if (allocator) {
    allocator->Free(p);
  } else {
    free(p);
  }
}

}