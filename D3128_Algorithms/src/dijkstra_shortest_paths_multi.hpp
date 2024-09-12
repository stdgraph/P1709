template <index_adjacency_list G,
          input_range          Sources,
          random_access_range  Distances,
          random_access_range  Predecessors,
          class WF      = function<range_value_t<Distances>(edge_reference_t<G>)>,
          class Visitor = empty_visitor,
          class Compare = less<range_value_t<Distances>>,
          class Combine = plus<range_value_t<Distances>>>
requires convertible_to<range_value_t<Sources>, vertex_id_t<G>> &&
         is_arithmetic_v<range_value_t<Distances>> &&              
         sized_range<Distances> &&                                 
         sized_range<Predecessors> &&                              
         convertible_to<vertex_id_t<G>, range_value_t<Predecessors>> &&
         basic_edge_weight_function<G, WF, range_value_t<Distances>, Compare, Combine>
constexpr void dijkstra_shortest_paths(
      G&&            g,
      const Sources& sources,
      Distances&     distances,
      Predecessors&  predecessor,
      WF&&      weight  = [](edge_reference_t<G> uv) { return range_value_t<Distances>(1); },
      Visitor&& visitor = empty_visitor(),
      Compare&& compare = less<range_value_t<Distances>>(),
      Combine&& combine = plus<range_value_t<Distances>>());
