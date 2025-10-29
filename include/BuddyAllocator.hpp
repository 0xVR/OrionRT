#ifndef TENSORPOOL_BUDDY_ALLOCATOR_HPP_
#define TENSORPOOL_BUDDY_ALLOCATOR_HPP_

#include <cstddef>

class BuddyAllocator {
 public:
  BuddyAllocator(void* base, size_t total_bytes, size_t alignment);

  void* Allocate(size_t bytes);
  void Release(void* ptr, size_t bytes);

 private:
  void* base_;
  size_t total_bytes_;
  size_t alignment_;
};

#endif  // TENSORPOOL_BUDDY_ALLOCATOR_HPP_
