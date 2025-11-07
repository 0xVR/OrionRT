#ifndef TENSORPOOL_REUSE_PLANNER_HPP_
#define TENSORPOOL_REUSE_PLANNER_HPP_

#include <unordered_map>
#include <vector>

#include "Arena.hpp"
#include "MemoryInterval.hpp"

class ReusePlanner {
 public:
  explicit ReusePlanner(Arena& arena);

  std::unordered_map<int, void*> Plan(
      const std::vector<MemoryInterval>& intervals);

 private:
  Arena& arena_;

  struct Active {
    int id;
    void* ptr;
    size_t bytes;
    int end;
  };

  std::vector<Active> active_;
};

#endif  // TENSORPOOL_REUSE_PLANNER_HPP_
