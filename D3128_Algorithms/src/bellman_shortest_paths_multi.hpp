template <adjacency_list G,
          input_range    Sources,
          class          Distances,
          class          Predecessors,
          class WF      = function<vertex_property_map_value_t<Distances>(const remove_reference_t<G>&,
                                                                          const edge_t<G>&)>,
          class Visitor = empty_visitor,
          class Compare = less<vertex_property_map_value_t<Distances>>,
          class Combine = plus<vertex_property_map_value_t<Distances>>>
requires vertex_property_map_for<Distances, G> &&
         vertex_property_map_for<Predecessors, G> &&
         convertible_to<range_value_t<Sources>, vertex_id_t<G>> &&
         is_arithmetic_v<vertex_property_map_value_t<Distances>> &&
         convertible_to<vertex_id_t<G>, vertex_property_map_value_t<Predecessors>> &&
         basic_edge_weight_function<G, WF, vertex_property_map_value_t<Distances>, Compare, Combine>
[[nodiscard]] constexpr optional<vertex_id_t<G>> bellman_ford_shortest_paths(
      G&&            g,
      const Sources& sources,
      Distances&     distances,
      Predecessors&  predecessor,
      WF&&           weight  = [](const auto&,
                       const edge_t<G>& uv) { return vertex_property_map_value_t<Distances>(1); },
      Visitor&&      visitor = empty_visitor(),
      Compare&&      compare = less<vertex_property_map_value_t<Distances>>(),
      Combine&&      combine = plus<vertex_property_map_value_t<Distances>>());
