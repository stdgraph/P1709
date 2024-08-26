template <index_adjacency_list        G,
          ranges::random_access_range Distances,
          class WF      = std::function<ranges::range_value_t<Distances>(edge_reference_t<G>)>,
          class Visitor = bellman_visitor_base<G>,
          class Compare = less<ranges::range_value_t<Distances>>,
          class Combine = plus<ranges::range_value_t<Distances>>>
requires is_arithmetic_v<ranges::range_value_t<Distances>> && //
         basic_edge_weight_function<G, WF, ranges::range_value_t<Distances>, Compare, Combine> &&
         bellman_visitor<G, Visitor>
bool bellman_ford_shortest_distances(
      G&                   g,
      const vertex_id_t<G> source,
      Distances&           distances,
      WF&&                 weight =
            [](edge_reference_t<G> uv) { return ranges::range_value_t<Distances>(1); },
      Visitor&& visitor = bellman_visitor_base<G>(),
      Compare&& compare = less<ranges::range_value_t<Distances>>(),
      Combine&& combine = plus<ranges::range_value_t<Distances>>());
