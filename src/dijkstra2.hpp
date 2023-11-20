// Ranges concepts and types don't include the ranges:: namespace prefix for brevity 
// and clarity of purpose

template <adjacency_list              G, 
          random_access_range Distance,
          random_access_range Predecessor
          class WF = function<range_value_t<Distance>(edge_reference_t<G>)>>
requires random_access_range<vertex_range_t<G>> &&
         integral<vertex_id_t<G>> &&
         is_arithmetic_v<range_value_t<Distance>> &&
         convertible_to<vertex_id_t<G>, range_value_t<Predecessor>> &&
         edge_weight_function<G, WF>
void dijkstra_shortest_paths(
      const G&       g, 
      vertex_id_t<G> source, 
      Distance&      distances, 
      Predecessor&   predecessors, 
      WF&& w = [](edge_reference_t<G> uv) { return range_value_t<Distance>(1); });

template <adjacency_list      G, 
          random_access_range Distance,
          random_access_range Predecessor,
          class               Compare, 
          class               Combine,
          class WF = function<range_value_t<Distance>(edge_reference_t<G>)>>
requires random_access_range<vertex_range_t<G>> &&
         integral<vertex_id_t<G>> &&
         is_arithmetic_v<range_value_t<Distance>> &&
         convertible_to<vertex_id_t<G>, range_value_t<Predecessor>> &&
         edge_weight_function<G, WF> &&
         strict_weak_order<Compare, range_value_t<Distance>, range_value_t<Distance> &&
         assignable_from <range_reference_t<Distance>,
            invoke_result_t <Combine, invoke_result_t<WF, edge_t<G>>,
void dijkstra_shortest_paths(
      const G&       g, 
      vertex_id_t<G> source, 
      Distance&      distances, 
      Predecessor&   predecessors, 
      WF&&           w,
      Compare&&      comp,
      Combine&&      comb);
