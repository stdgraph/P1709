// Concepts and types from std::ranges don't include the namespace prefix for brevity 
// and clarity of purpose

// A fake range with no values and does nothing
inline static null_range_type null_distances;

template <index_adjacency_list G, 
          random_access_range  Distance,
          random_access_range  Predecessor
          class WF = function<range_value_t<Distance>(edge_reference_t<G>)>>
requires convertible_to<vertex_id_t<G>, range_value_t<Predecessor>> &&
         edge_weight_function<G, WF, range_value_t<Distance>>
void dijkstra_shortest_paths(
      const G&       graph, 
      vertex_id_t<G> source, 
      Predecessor&   predecessors, 
      Distance&      distances = null_distances, 
      WF&& w = [](edge_reference_t<G> uv) { return range_value_t<Distance>(1); });

template <index_adjacency_list G, 
          random_access_range  Distance,
          random_access_range  Predecessor,
          class                Compare, 
          class                Combine,
          class WF = function<range_value_t<Distance>(edge_reference_t<G>)>>
requires convertible_to<vertex_id_t<G>, range_value_t<Predecessor>> &&
         basic_edge_weight_function<G, WF, range_value_t<Distance>, Compare, Combine>
void dijkstra_shortest_paths(
      const G&       graph, 
      vertex_id_t<G> source, 
      Compare&&      compare,
      Combine&&      combare
      Predecessor&   predecessors, 
      Distance&      distances = null_distances, 
      WF&& w = [](edge_reference_t<G> uv) { return range_value_t<Distance>(1); });
