#include "BuddyAllocator.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>

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

size_t LevelSizeStatic(int level) {
  return (1ULL << level);
}

// Split a block down to the needed level.
static void* SplitBlockStatic(std::list<BuddyAllocator::Block>* free_lists,
                              int from_level,
                              int to_level,
                              void* base) {
  if (from_level == to_level) {
    auto block = free_lists[from_level].front();
    free_lists[from_level].pop_front();
    return block.ptr;
  }

  // Recursively split parent block
  void* parent = SplitBlockStatic(free_lists, from_level + 1, to_level, base);
  if (!parent) return nullptr;

  size_t size = LevelSizeStatic(to_level);
  void* buddy = static_cast<char*>(parent) + size;

  // Add buddy to free list
  free_lists[to_level].push_back({buddy, size});

  return parent;
}

void* BuddyAllocator::Allocate(size_t bytes) {
  int level = LevelForSizeStatic(bytes, alignment_);

  for (int i = level; i <= max_level_; i++) {
    if (!free_lists_[i].empty()) {
      // Split free block at i down to requested level
      return SplitBlockStatic(free_lists_, i, level, base_);
    }
  }
  return nullptr;
}

void BuddyAllocator::Release(void* ptr, size_t bytes) {
  // merge logic added in next commit
  free_lists_[LevelForSizeStatic(bytes, alignment_)].push_back(
      {ptr, LevelSizeStatic(LevelForSizeStatic(bytes, alignment_))});
}
