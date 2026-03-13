template <adjacency_list G,
          class          Predecessor,
          class          Weight>
requires vertex_property_map_for<Predecessor, G> &&
         vertex_property_map_for<Weight, G>
void prim(G&& g, Predecessor& predecessor, Weight& weight, const vertex_id_t<G>& source = 0);

template <adjacency_list G,
          class          Predecessor,
          class          Weight,
          class CompareOp,
          class WF>
requires vertex_property_map_for<Predecessor, G> &&
         vertex_property_map_for<Weight, G> &&
         basic_edge_weight_function<G, WF, vertex_property_map_value_t<Weight>, CompareOp, plus<vertex_property_map_value_t<Weight>>>
void prim(G&&                                 g,
          Predecessor&                        predecessor,
          Weight&                             weight,
          CompareOp                           compare,
          vertex_property_map_value_t<Weight> init_dist,
          WF&&                                weight_fn,
          vertex_id_t<G>                      source = 0);