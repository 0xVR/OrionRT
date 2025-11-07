#include "ReusePlanner.hpp"

ReusePlanner::ReusePlanner(Arena& arena) : arena_(arena) {}

std::unordered_map<int, void*> ReusePlanner::Plan(
    const std::vector<MemoryInterval>& intervals) {
  // Stub for now
  return {};
}
