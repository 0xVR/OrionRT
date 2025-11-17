# OrionRT

OrionRT is a lightweight C++17 runtime allocator designed for workloads with
static execution graphs, such as deep-learning inference pipelines. It performs
lifetime analysis over a sequence of operations and assigns each tensor a
physical memory region using planned reuse. This reduces allocation overhead and
helps keep peak memory usage low.

The allocator backend is a classic buddy allocator with power-of-two block
splitting and coalescing. A linear-scan reuse planner maps non-overlapping
tensor lifetimes to shared memory regions within the arena.

---

## Key Features

- **Graph-aware lifetime tracking**  
  Computes first-use and last-use indices for every tensor.

- **Interval-based memory reuse**  
  Assigns tensors to memory blocks using a single-pass linear-scan planner.

- **Buddy allocator backend**  
  Power-of-two splitting and XOR-based buddy merging for contiguous allocations.

- **Deterministic, low-overhead runtime**  
  All physical assignments are precomputed; runtime "allocations" require no
  system calls.

---

## Architecture Overview

- **GraphLiveness**  
  Produces a liveness interval `[start, end]` for each tensor.

- **ReusePlanner**  
  Reuses memory from intervals that have already expired.

- **BuddyAllocator**  
  Manages the underlying arena using classic buddy allocation semantics.

- **TensorPool**  
  Orchestrates analysis, planning, and runtime access.

---

## Example

```cpp
TensorPool pool(64 * 1024 * 1024);  // 64 MB arena

std::vector<OpNode> ops = {
    {0, {},   {0}},
    {1, {0},  {1}},
    {2, {1},  {2}},
    {3, {2},  {}},
};

std::vector<TensorDesc> descs = {
    {0, 1024},
    {1, 2048},
    {2, 1024},
};

pool.RegisterGraph(ops, descs);
pool.FinalizePlan();

auto h = pool.Allocate(1);
std::cout << "Tensor 1 at " << h.ptr << ", bytes=" << h.bytes << "\n";
````

---

## Building

```bash
mkdir build && cd build
cmake ..
make -j
```

---

## Design Notes

* TensorPool assumes a fixed, linear execution order.
* All memory planning is performed ahead of execution.
* The buddy allocator uses a single contiguous arena.
* The reuse planner uses simple first-fit behavior; other heuristics could be added without changing the interface.

---

## Possible Extensions

* Multi-arena or per-device allocation strategies
* Alignment-aware suballocation for GPU workloads
* Asynchronous lifetime tracking
* Fragmentation metric reporting and visualization
* Custom allocation policies in place of linear scan


