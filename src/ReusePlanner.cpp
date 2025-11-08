#include "ReusePlanner.hpp"

#include <algorithm>

ReusePlanner::ReusePlanner(Arena& arena) : arena_(arena) {}

std::unordered_map<int, void*> ReusePlanner::Plan(
    const std::vector<MemoryInterval>& intervals_in) {
  std::unordered_map<int, void*> result;

  // Sort by start time
  auto intervals = intervals_in;
  std::sort(intervals.begin(), intervals.end(),
            [](const MemoryInterval& a, const MemoryInterval& b) {
              return a.start < b.start;
            });

  for (const auto& iv : intervals) {
    // Drop expired active intervals
    active_.erase(std::remove_if(active_.begin(), active_.end(),
                                 [&](const Active& a) {
                                   if (a.end < iv.start) {
                                     arena_.Release(a.ptr, a.bytes);
                                     return true;
                                   }
                                   return false;
                                 }),
                  active_.end());

    // Allocate fresh block (buddy allocator handles reuse)
    void* ptr = arena_.Allocate(iv.bytes);

    result[iv.tensor_id] = ptr;

    active_.push_back({iv.tensor_id, ptr, iv.bytes, iv.end});
  }

  return result;
}
