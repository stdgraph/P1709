template <class Predecessors>
constexpr void init_topological_sort(Predecessors& predecessors) {
  // exposition only
  size_t i = 0;
  for(auto& pred : predecessors)
    pred = i++;      
}
