template <index_adjacency_list        G,
          ranges::random_access_range Distances,
          ranges::random_access_range Predecessors,
          class WF      = std::function<ranges::range_value_t<Distances>(edge_reference_t<G>)>,
          class Visitor = bellman_visitor_base<G>,
          class Compare = less<ranges::range_value_t<Distances>>,
          class Combine = plus<ranges::range_value_t<Distances>>>
requires is_arithmetic_v<ranges::range_value_t<Distances>> &&                   
         convertible_to<vertex_id_t<G>, ranges::range_value_t<Predecessors>> && 
         ranges::sized_range<Distances> &&                                      
         ranges::sized_range<Predecessors> &&                                   
         basic_edge_weight_function<G, WF, ranges::range_value_t<Distances>, Compare, Combine> &&
         bellman_visitor<G, Visitor>
bool bellman_ford_shortest_paths(
      G&                   g,
      const vertex_id_t<G> source,
      Distances&           distances,
      Predecessors&        predecessor,
      WF&&      weight = [](edge_reference_t<G> uv) { return ranges::range_value_t<Distances>(1); },
      Visitor&& visitor = bellman_visitor_base<G>(),
      Compare&& compare = less<ranges::range_value_t<Distances>>(),
      Combine&& combine = plus<ranges::range_value_t<Distances>>());
