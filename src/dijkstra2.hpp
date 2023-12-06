// Concepts and types from std::ranges don't include the namespace prefix for brevity 
// and clarity of purpose

template <class G>
concept index_adjacency_list = 
      adjacency_list<G> && //
      random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>;





template <class G, class WF, class DistVal, class Compare, class Combine>
concept basic_edge_weight_function = // e.g. weight(uv)
      is_arithmetic_v<DistVal> &&
      strict_weak_order<Compare, DistVal, DistVal> &&
      assignable_from<add_lvalue_reference_t<DistVal>, 
                      invoke_result_t<Combine, invoke_result_t<WF, edge_reference_t<G>>>>;

template <class G, class WF, class DistVal>
concept edge_weight_function = // e.g. weight(uv)
      is_arithmetic_v<invoke_result_t<WF, edge_reference_t<G>>> &&
      basic_edge_weight_function<G,
                                  WF,
                                  Distance,
                                  less<DistVal>,
                                  plus<DistVal>>;


template <index_adjacency_list G, 
          random_access_range  Distance,
          random_access_range  Predecessor
          class WF = function<range_value_t<Distance>(edge_reference_t<G>)>>
requires convertible_to<vertex_id_t<G>, range_value_t<Predecessor>> &&
         edge_weight_function<G, WF, range_value_t<Distance>>
void dijkstra_shortest_paths(
      const G&       graph, 
      vertex_id_t<G> source, 
      Distance&      distances, 
      Predecessor&   predecessors, 
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
      Distance&      distances, 
      Predecessor&   predecessors, 
      WF&&           w,
      Compare&&      compare,
      Combine&&      combare);
