#include "BuddyAllocator.hpp"

BuddyAllocator::BuddyAllocator(void* base,
                               size_t total_bytes,
                               size_t alignment)
    : base_(base),
      total_bytes_(total_bytes),
      alignment_(alignment) {}

void* BuddyAllocator::Allocate(size_t bytes) {
  // stub for now
  return nullptr;
}

void BuddyAllocator::Release(void* ptr, size_t bytes) {
  // stub
}
