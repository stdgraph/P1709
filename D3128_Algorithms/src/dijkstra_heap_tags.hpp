// Heap-selector tag: use std::priority_queue with lazy deletion (default).
// Heap entries may grow to O(E); stale entries are skipped at pop time.
struct use_default_heap {};

// Heap-selector tag: use an indexed d-ary heap with true decrease-key.
// Heap size is bounded by O(V); no stale pops.
// @tparam Arity  Children per node (default 4, matching Boost's d_ary_heap_indirect).
template <size_t Arity = 4>
struct use_indexed_dary_heap {
  static constexpr size_t arity = Arity;
};
