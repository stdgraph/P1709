template <index_adjacency_list G, 
          random_access_range  Distances,
          random_access_range  Predecessors,
          class                Compare, 
          class                Combine,
          class WF = function<range_value_t<Distances>(edge_reference_t<G>)>>
requires convertible_to<vertex_id_t<G>, range_value_t<Predecessors>> &&
         basic_edge_weight_function<G, WF, range_value_t<Distances>, Compare, Combine>
void bellman_ford_shortest_paths(
      const G&       graph, 
      vertex_id_t<G> source, 
      Distances&     distances, 
      Predecessors&  predecessors, 
      Compare&&      compare,
      Combine&&      combine,
      WF&& w = [](edge_reference_t<G> uv) { return range_value_t<Distances>(1); });

template <index_adjacency_list G, 
          random_access_range  Distances,
          class                Compare, 
          class                Combine,
          class WF = function<range_value_t<Distances>(edge_reference_t<G>)>>
requires basic_edge_weight_function<G, WF, range_value_t<Distances>, Compare, Combine>
void bellman_ford_shortest_distances(
      const G&       graph, 
      vertex_id_t<G> source, 
      Distances&     distances, 
      Compare&&      compare,
      Combine&&      combine,
      WF&& w = [](edge_reference_t<G> uv) { return range_value_t<Distances>(1); });

