template <index_adjacency_list G,
          random_access_range  Distances,
          random_access_range  Predecessors,
          class WF      = function<range_value_t<Distances>(edge_reference_t<G>)>,
          class Visitor = dijkstra_visitor_base<G>,
          class Compare = less<range_value_t<Distances>>,
          class Combine = plus<range_value_t<Distances>>>
requires is_arithmetic_v<range_value_t<Distances>> &&
         sized_range<Distances> &&                    
         sized_range<Predecessors> &&                 
         convertible_to<vertex_id_t<G>, range_value_t<Predecessors>> &&
         basic_edge_weight_function<G, WF, range_value_t<Distances>, Compare, Combine> && 
         dijkstra_visitor<G, Visitor>
constexpr void dijkstra_shortest_paths(
      G&                   g,
      const vertex_id_t<G> source,
      Distances&           distances,
      Predecessors&        predecessor,
      WF&&      weight  = [](edge_reference_t<G> uv) { return range_value_t<Distances>(1); },
      Visitor&& visitor = dijkstra_visitor_base<G>(),
      Compare&& compare = less<range_value_t<Distances>>(),
      Combine&& combine = plus<range_value_t<Distances>>());