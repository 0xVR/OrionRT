#include <iostream>

#include "TensorPool.hpp"

int main() {
  TensorPool pool(64 * 1024 * 1024);  // 64 MB arena

  std::vector<OpNode> ops = {
      {0, {}, {0}},
      {1, {0}, {1}},
      {2, {1}, {2}},
      {3, {2}, {}},
  };

  std::vector<TensorDesc> descs = {
      {0, 1024},
      {1, 2048},
      {2, 1024},
  };

  pool.RegisterGraph(ops, descs);
  pool.FinalizePlan();

  for (int i = 0; i < 3; i++) {
    auto h = pool.Allocate(i);
    std::cout << "Tensor " << i << " → ptr=" << h.ptr
              << " bytes=" << h.bytes << std::endl;
  }

  std::cout << "Peak memory = " << pool.peak_memory() << " bytes\n";
}
