#include "GraphLiveness.hpp"

GraphLiveness::GraphLiveness(const std::vector<OpNode>& ops,
                             const std::vector<TensorDesc>& tensors)
    : ops_(ops), tensors_(tensors) {}

void GraphLiveness::ComputeLifetimes() {
  for (size_t i = 0; i < ops_.size(); i++) {
    const auto& op = ops_[i];

    for (int t : op.inputs) {
      for (auto& desc : tensors_) {
        if (desc.id == t) {
          if (desc.first_use < 0) desc.first_use = i;
          desc.last_use = i;
        }
      }
    }

    for (int t : op.outputs) {
      for (auto& desc : tensors_) {
        if (desc.id == t) {
          if (desc.first.first_use < 0) desc.first_use = i;
          desc.last_use = i;
        }
      }
    }
  }
}
