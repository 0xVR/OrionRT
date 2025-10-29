#ifndef TENSORPOOL_ARENA_HPP_
#define TENSORPOOL_ARENA_HPP_

#include <cstddef>
#include <cstdint>

class BuddyAllocator;

class Arena {
 public:
  explicit Arena(size_t total_bytes, size_t alignment = 256);

  void* Allocate(size_t bytes);
  void Release(void* ptr, size_t bytes);

  size_t peak() const { return peak_bytes_; }
  size_t capacity() const { return total_bytes_; }

 private:
  size_t total_bytes_;
  size_t alignment_;
  void* base_ptr_;

  BuddyAllocator* allocator_;

  size_t peak_bytes_ = 0;
};

#endif  // TENSORPOOL_ARENA_HPP_
