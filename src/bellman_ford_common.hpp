template <index_adjacency_list G, 
          random_access_range  Distances,
          random_access_range  Predecessors
          class WF = function<range_value_t<Distances>(edge_reference_t<G>)>>
requires convertible_to<vertex_id_t<G>, range_value_t<Predecessors>> &&
         edge_weight_function<G, WF, range_value_t<Distances>>
void bellman_ford_shortest_paths(
      const G&       graph, 
      vertex_id_t<G> source, 
      Distances&     distancess, 
      Predecessors&  predecessors, 
      WF&& w = [](edge_reference_t<G> uv) { return range_value_t<Distances>(1); });

template <index_adjacency_list G, 
          random_access_range  Distances,
          class WF = function<range_value_t<Distances>(edge_reference_t<G>)>>
requires edge_weight_function<G, WF, range_value_t<Distances>>
void bellman_ford_shortest_paths(
      const G&       graph, 
      vertex_id_t<G> source, 
      Distances&     distancess, 
      WF&& w = [](edge_reference_t<G> uv) { return range_value_t<Distances>(1); });
