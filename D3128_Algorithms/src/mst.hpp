/*
 * Kruskal's Algorithm
 */
template <x_index_edgelist_range IELR, x_index_edgelist_range OELR>
auto kruskal(IELR&& e, OELR&& t);

template <x_index_edgelist_range IELR, x_index_edgelist_range OELR, class CompareOp>
auto kruskal(IELR&& e, OELR&& t, CompareOp compare);

/*
 * Inplace Kruskal's Algorithm
 */
template <x_index_edgelist_range IELR, x_index_edgelist_range OELR>
requires permutable<iterator_t<IELR>>
auto inplace_kruskal(IELR&& e, OELR&& t);

template <x_index_edgelist_range IELR, x_index_edgelist_range OELR, class CompareOp>
requires permutable<iterator_t<IELR>>
auto inplace_kruskal(IELR&& e, OELR&& t, CompareOp compare);

/*
 * Prim's Algorithm
 */
template <adjacency_list G, class Predecessor, class Weight>
auto prim(G&& g, Predecessor& predecessor, Weight& weight, const vertex_id_t<G>& seed = 0);

template <adjacency_list G,
          class          Predecessor,
          class          Weight,
          class CompareOp,
          class WF>
requires basic_edge_weight_function<G,
                                    WF,
                                    vertex_property_map_value_t<Weight>,
                                    CompareOp,
                                    plus<vertex_property_map_value_t<Weight>>>
auto prim(G&&                                 g,
          Predecessor&                        predecessor,
          Weight&                             weight,
          CompareOp                           compare,
          vertex_property_map_value_t<Weight> init_dist,
          WF&&                                weight_fn,
          vertex_id_t<G>                      seed = 0);
