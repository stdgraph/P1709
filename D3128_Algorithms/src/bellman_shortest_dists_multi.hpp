template <index_adjacency_list G,
          input_range          Sources,
          random_access_range  Distances,
          class WF      = function<range_value_t<Distances>(edge_reference_t<G>)>,
          class Visitor = bellman_visitor_base<G>,
          class Compare = less<range_value_t<Distances>>,
          class Combine = plus<range_value_t<Distances>>>
requires convertible_to<range_value_t<Sources>, vertex_id_t<G>> && 
         is_arithmetic_v<range_value_t<Distances>> &&              
         sized_range<Distances> &&                                 
         basic_edge_weight_function<G, WF, range_value_t<Distances>, Compare, Combine> && 
         bellman_visitor<G, Visitor>
constexpr optional<vertex_id_t<G>> bellman_ford_shortest_distances(
      G&             g,
      const Sources& sources,
      Distances&     distances,
      WF&&      weight  = [](edge_reference_t<G> uv) { return range_value_t<Distances>(1); },
      Visitor&& visitor = bellman_visitor_base<G>(),
      Compare&& compare = less<range_value_t<Distances>>(),
      Combine&& combine = plus<range_value_t<Distances>>());
