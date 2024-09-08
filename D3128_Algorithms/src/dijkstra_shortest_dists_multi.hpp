template <index_adjacency_list G,
          input_range          Sources,
          random_access_range  Distances,
          class WF      = function<range_value_t<Distances>(edge_reference_t<G>)>,
          class Visitor = dijkstra_visitor_base<G>,
          class Compare = less<range_value_t<Distances>>,
          class Combine = plus<range_value_t<Distances>>>
requires convertible_to<range_value_t<Sources>, vertex_id_t<G>> &&
         sized_range<Distances> &&                                 
         is_arithmetic_v<range_value_t<Distances>> &&              
         basic_edge_weight_function<G, WF, range_value_t<Distances>, Compare, Combine> && 
         dijkstra_visitor<G, Visitor>
constexpr void dijkstra_shortest_distances(
      G&             g,
      const Sources& sources,
      Distances&     distances,
      WF&&      weight  = [](edge_reference_t<G> uv) { return range_value_t<Distances>(1); },
      Visitor&& visitor = dijkstra_visitor_base<G>(),
      Compare&& compare = less<range_value_t<Distances>>(),
      Combine&& combine = plus<range_value_t<Distances>>());
