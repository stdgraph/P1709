template <class Predecessors>
constexpr void init_breadth_first_search(Predecessors& predecessors) {
  // exposition only
  size_t i = 0;
  for(auto& pred : predecessors)
    pred = i++;      
}
