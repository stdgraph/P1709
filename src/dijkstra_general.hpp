template <index_adjacency_list G, 
          random_access_range  Distances,
          class                Compare, 
          class                Combine,
          class WF = function<range_value_t<Distances>(edge_reference_t<G>)>>
requires basic_edge_weight_function<G, WF, range_value_t<Distances>, Compare, Combine>
void dijkstra_shortest_paths(
      const G&       graph, 
      vertex_id_t<G> source, 
      Distance&      distances, 
      Compare&&      compare,
      Combine&&      combine,
      WF&& w = [](edge_reference_t<G> uv) { return range_value_t<Distance>(1); });

template <index_adjacency_list G, 
          random_access_range  Distances,
          random_access_range  Predecessors,
          class                Compare, 
          class                Combine,
          class WF = function<range_value_t<Distances>(edge_reference_t<G>)>>
requires convertible_to<vertex_id_t<G>, range_value_t<Predecessor>> &&
         basic_edge_weight_function<G, WF, range_value_t<Distance>, Compare, Combine>
void dijkstra_shortest_paths(
      const G&       graph, 
      vertex_id_t<G> source, 
      Distances&     distances, 
      Predecessors&  predecessors, 
      Compare&&      compare,
      Combine&&      combine,
      WF&& w = [](edge_reference_t<G> uv) { return range_value_t<Distance>(1); });
