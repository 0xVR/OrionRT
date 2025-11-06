#ifndef TENSORPOOL_MEMORY_INTERVAL_HPP_
#define TENSORPOOL_MEMORY_INTERVAL_HPP_

#include <cstddef>

struct MemoryInterval {
  int tensor_id;
  size_t bytes;
  int start;
  int end;
  void* assigned = nullptr;
};

#endif  // TENSORPOOL_MEMORY_INTERVAL_HPP_
