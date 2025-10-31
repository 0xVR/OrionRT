#include "BuddyAllocator.hpp"

BuddyAllocator::BuddyAllocator(void* base,
                               size_t total_bytes,
                               size_t alignment)
    : base_(base),
      total_bytes_(total_bytes),
      alignment_(alignment),
      max_level_(0) {
  while ((1ULL << max_level_) < total_bytes_) {
    max_level_++;
  }
  max_level_--;

  free_lists_[max_level_].push_back({base_, total_bytes_});
}

void* BuddyAllocator::Allocate(size_t bytes) {
  return nullptr;  // not implemented yet
}

void BuddyAllocator::Release(void* ptr, size_t bytes) {
  // noop for now
}
