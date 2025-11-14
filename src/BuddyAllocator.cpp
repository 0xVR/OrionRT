#include "BuddyAllocator.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>

static int LevelForSizeStatic(size_t size, size_t alignment) {
  size = std::max(size, alignment);
  int level = 0;
  while ((1ULL << level) < size) level++;
  return level;
}

static size_t LevelSizeStatic(int level) {
  return (1ULL << level);
}

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

static void* SplitBlockStatic(std::list<BuddyAllocator::Block>* free_lists,
                              int from_level,
                              int to_level,
                              void* /*base*/) {
  if (from_level == to_level) {
    auto block = free_lists[from_level].front();
    free_lists[from_level].pop_front();
    return block.ptr;
  }

  void* parent = SplitBlockStatic(free_lists, from_level + 1, to_level, nullptr);
  if (!parent) return nullptr;

  size_t size = LevelSizeStatic(to_level);
  void* buddy = static_cast<char*>(parent) + size;

  free_lists[to_level].push_back({buddy, size});

  return parent;
}

static void* BuddyOf(void* base, void* ptr, size_t size) {
  size_t offset =
      reinterpret_cast<char*>(ptr) - reinterpret_cast<char*>(base);
  size_t buddy_offset = offset ^ size;
  return reinterpret_cast<char*>(base) + buddy_offset;
}

void* BuddyAllocator::Allocate(size_t bytes) {
  int level = LevelForSizeStatic(bytes, alignment_);

  for (int i = level; i <= max_level_; i++) {
    if (!free_lists_[i].empty()) {
      return SplitBlockStatic(free_lists_, i, level, base_);
    }
  }
  return nullptr;
}

void BuddyAllocator::Release(void* ptr, size_t bytes) {
  int level = LevelForSizeStatic(bytes, alignment_);
  size_t size = LevelSizeStatic(level);

  void* current = ptr;

  for (;;) {
    void* buddy = BuddyOf(base_, current, size);
    auto& fl = free_lists_[level];

    bool found = false;
    for (auto it = fl.begin(); it != fl.end(); ++it) {
      if (it->ptr == buddy) {
        fl.erase(it);
        current = std::min(current, buddy);
        size <<= 1;
        level++;
        found = true;
        break;
      }
    }

    if (!found || level > max_level_) break;
  }

  free_lists_[level].push_back({current, size});
}
