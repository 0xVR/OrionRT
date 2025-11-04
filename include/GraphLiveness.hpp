#ifndef TENSORPOOL_GRAPH_LIVENESS_HPP_
#define TENSORPOOL_GRAPH_LIVENESS_HPP_

#include <vector>
#include <cstddef>

struct OpNode {
  int id;
  std::vector<int> inputs;
  std::vector<int> outputs;
};

struct TensorDesc {
  int id;
  size_t bytes;
  int first_use = -1;
  int last_use = -1;
};

class GraphLiveness {
 public:
  GraphLiveness(const std::vector<OpNode>& ops,
                const std::vector<TensorDesc>& tensors);

  void ComputeLifetimes();
  const std::vector<TensorDesc>& tensors() const { return tensors_; }

 private:
  std::vector<OpNode> ops_;
  std::vector<TensorDesc> tensors_;
};

#endif  // TENSORPOOL_GRAPH_LIVENESS_HPP_
