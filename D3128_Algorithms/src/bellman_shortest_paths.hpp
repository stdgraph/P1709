template <index_adjacency_list G,
          random_access_range  Distances,
          random_access_range  Predecessors,
          class WF      = function<range_value_t<Distances>(edge_reference_t<G>)>,
          class Visitor = bellman_visitor_base<G>,
          class Compare = less<range_value_t<Distances>>,
          class Combine = plus<range_value_t<Distances>>>
requires is_arithmetic_v<range_value_t<Distances>> &&                   
         convertible_to<vertex_id_t<G>, range_value_t<Predecessors>> && 
         sized_range<Distances> &&                                      
         sized_range<Predecessors> &&                                   
         basic_edge_weight_function<G, WF, range_value_t<Distances>, Compare, Combine> && 
         bellman_visitor<G, Visitor>
optional<vertex_id_t<G>> bellman_ford_shortest_paths(
      G&                   g,
      const vertex_id_t<G> source,
      Distances&           distances,
      Predecessors&        predecessor,
      WF&&      weight  = [](edge_reference_t<G> uv) { return range_value_t<Distances>(1); },
      Visitor&& visitor = bellman_visitor_base<G>(),
      Compare&& compare = less<range_value_t<Distances>>(),
      Combine&& combine = plus<range_value_t<Distances>>());
