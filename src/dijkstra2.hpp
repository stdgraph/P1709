// Concepts and types from std::ranges don't include the namespace prefix for brevity 
// and clarity of purpose

template <class G, class WF, class Distance, class Compare, class Combine>
concept basic_edge_weight_function2 = // e.g. weight(uv)
      is_arithmetic_v<range_value_t<Distance>> &&
      strict_weak_order<Compare, range_value_t<Distance>, range_value_t<Distance>> &&
      assignable_from<range_reference_t<Distance>, 
                      invoke_result_t<Combine, invoke_result_t<WF, edge_reference_t<G>>>>;

template <class G, class WF, class Distance>
concept edge_weight_function2 = // e.g. weight(uv)
      is_arithmetic_v<invoke_result_t<WF, edge_reference_t<G>>> &&
      basic_edge_weight_function2<G,
                                  WF,
                                  Distance,
                                  less<range_value_t<Distance>>,
                                  plus<range_value_t<Distance>>>;


template <index_adjacency_list G, 
          random_access_range  Distance,
          random_access_range  Predecessor
          class WF = function<range_value_t<Distance>(edge_reference_t<G>)>>
requires convertible_to<vertex_id_t<G>, range_value_t<Predecessor>> &&
         edge_weight_function2<G, WF, Distance>
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
         basic_edge_weight_function2<G, WF, Distance, Compare, Combine>
void dijkstra_shortest_paths(
      const G&       graph, 
      vertex_id_t<G> source, 
      Distance&      distances, 
      Predecessor&   predecessors, 
      WF&&           w,
      Compare&&      comp,
      Combine&&      comb);
