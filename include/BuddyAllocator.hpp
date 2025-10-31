#ifndef TENSORPOOL_BUDDY_ALLOCATOR_HPP_
#define TENSORPOOL_BUDDY_ALLOCATOR_HPP_

#include <cstddef>
#include <list>

class BuddyAllocator {
 public:
  BuddyAllocator(void* base, size_t total_bytes, size_t alignment);

  void* Allocate(size_t bytes);
  void Release(void* ptr, size_t bytes);

 private:
  struct Block {
    void* ptr;
    size_t size;
  };

  static constexpr int kMaxLevels = 32;
  std::list<Block> free_lists_[kMaxLevels];

  void* base_;
  size_t total_bytes_;
  size_t alignment_;
  int max_level_;
};

#endif  // TENSORPOOL_BUDDY_ALLOCATOR_HPP_
