template <index_adjacency_list        G,
          ranges::input_range         Sources,
          ranges::random_access_range Distances,
          ranges::random_access_range Predecessors,
          class WF      = function<ranges::range_value_t<Distances>(edge_reference_t<G>)>,
          class Visitor = dijkstra_visitor_base<G>,
          class Compare = less<ranges::range_value_t<Distances>>,
          class Combine = plus<ranges::range_value_t<Distances>>>
requires convertible_to<ranges::range_value_t<Sources>, vertex_id_t<G>> &&
         is_arithmetic_v<ranges::range_value_t<Distances>> &&
         convertible_to<vertex_id_t<G>, ranges::range_value_t<Predecessors>> &&
         basic_edge_weight_function<G, WF, ranges::range_value_t<Distances>, Compare, Combine>
void dijkstra_shortest_paths(
      G&             g,
      const Sources& sources,
      Distances&     distances,
      Predecessors&  predecessor,
      WF&&      weight  = [](edge_reference_t<G> uv) { return ranges::range_value_t<Distances>(1); },
      Visitor&& visitor = dijkstra_visitor_base<G>(),
      Compare&& compare = less<ranges::range_value_t<Distances>>(),
      Combine&& combine = plus<ranges::range_value_t<Distances>>());
