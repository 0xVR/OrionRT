#ifndef TENSORPOOL_TENSOR_POOL_HPP_
#define TENSORPOOL_TENSOR_POOL_HPP_

#include <unordered_map>
#include <vector>

#include "Arena.hpp"
#include "GraphLiveness.hpp"
#include "ReusePlanner.hpp"

struct TensorHandle {
  int id;
  void* ptr;
  size_t bytes;
};

class TensorPool {
 public:
  explicit TensorPool(size_t arena_bytes);

  void RegisterGraph(const std::vector<OpNode>& ops,
                     const std::vector<TensorDesc>& descs);

  void FinalizePlan();

  TensorHandle Allocate(int tensor_id);
  void Release(TensorHandle h);

  void* GetPtr(int tensor_id) const;

  size_t peak_memory() const { return arena_.peak(); }

 private:
  Arena arena_;

  std::vector<OpNode> ops_;
  std::vector<TensorDesc> tensors_;

  std::unordered_map<int, void*> assignment_;

  bool finalized_ = false;
};

#endif  // TENSORPOOL_TENSOR_POOL_HPP_
