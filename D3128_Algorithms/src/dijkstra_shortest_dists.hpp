template <adjacency_list G,
          class          Distances,
          class WF      = function<vertex_property_map_value_t<Distances>(const remove_reference_t<G>&,
                                                                          const edge_t<G>&)>,
          class Visitor = empty_visitor,
          class Compare = less<vertex_property_map_value_t<Distances>>,
          class Combine = plus<vertex_property_map_value_t<Distances>>>
requires vertex_property_map_for<Distances, G> &&
         is_arithmetic_v<vertex_property_map_value_t<Distances>> &&
         basic_edge_weight_function<G, WF, vertex_property_map_value_t<Distances>, Compare, Combine>
constexpr void dijkstra_shortest_distances(
      G&&                   g,
      const vertex_id_t<G>& source,
      Distances&            distances,
      WF&&                  weight  = [](const auto&,
                       const edge_t<G>& uv) { return vertex_property_map_value_t<Distances>(1); },
      Visitor&&             visitor = empty_visitor(),
      Compare&&             compare = less<vertex_property_map_value_t<Distances>>(),
      Combine&&             combine = plus<vertex_property_map_value_t<Distances>>());
