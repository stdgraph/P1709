template <class Predecessors>
constexpr void init_topological_sort(Predecessors& predecessors) {
  std::ranges::iota(predecessors, 0); // exposition only
}
