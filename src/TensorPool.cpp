#include "TensorPool.hpp"

TensorPool::TensorPool(size_t arena_bytes)
    : arena_(arena_bytes) {}

void TensorPool::RegisterGraph(const std::vector<OpNode>& ops,
                               const std::vector<TensorDesc>& descs) {
  ops_ = ops;
  tensors_ = descs;
}

void TensorPool::FinalizePlan() {
  GraphLiveness lv(ops_, tensors_);
  lv.ComputeLifetimes();
  tensors_ = lv.tensors();

  std::vector<MemoryInterval> intervals;
  intervals.reserve(tensors_.size());

  for (const auto& t : tensors_) {
    intervals.push_back({t.id, t.bytes, t.first_use, t.last_use});
  }

  ReusePlanner planner(arena_);
  assignment_ = planner.Plan(intervals);

  finalized_ = true;
}

TensorHandle TensorPool::Allocate(int tensor_id) {
  void* ptr = assignment_.at(tensor_id);

  for (const auto& t : tensors_) {
    if (t.id == tensor_id) {
      return TensorHandle{tensor_id, ptr, t.bytes};
    }
  }
  return {tensor_id, ptr, 0};
}

void TensorPool::Release(TensorHandle /*h*/) {
  // no-op: memory reused automatically
}

void* TensorPool::GetPtr(int tensor_id) const {
  auto it = assignment_.find(tensor_id);
  if (it != assignment_.end()) return it->second;
  return nullptr;
}
