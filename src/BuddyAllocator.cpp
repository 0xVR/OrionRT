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

static int LevelForSizeStatic(size_t size, size_t alignment) {
  size = std::max(size, alignment);
  int level = 0;
  while ((1ULL << level) < size) level++;
  return level;
}

void* BuddyAllocator::Allocate(size_t bytes) {
  int level = LevelForSizeStatic(bytes, alignment_);
  // splitting not implemented yet
  return nullptr;
}

void BuddyAllocator::Release(void* ptr, size_t bytes) {
  // noop for now
}
