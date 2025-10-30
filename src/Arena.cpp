#include "Arena.hpp"
#include "BuddyAllocator.hpp"

#include <cstdlib>
#include <algorithm>

Arena::Arena(size_t total_bytes, size_t alignment)
    : total_bytes_(total_bytes),
      alignment_(alignment),
      peak_bytes_(0) {
  base_ptr_ = std::aligned_alloc(alignment_, total_bytes_);
  allocator_ = new BuddyAllocator(base_ptr_, total_bytes_, alignment_);
}

void* Arena::Allocate(size_t bytes) {
  void* p = allocator_->Allocate(bytes);
  if (p) {
    peak_bytes_ = std::max(peak_bytes_, bytes);
  }
  return p;
}

void Arena::Release(void* ptr, size_t bytes) {
  allocator_->Release(ptr, bytes);
}
