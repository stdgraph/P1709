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
template <adjacency_list G,
          class          Predecessor,
          class          Weight,
          class WF      = function<vertex_property_map_value_t<Weight>(const remove_reference_t<G>&,
                                                                        const edge_t<G>&)>,
          class CompareOp = less<vertex_property_map_value_t<Weight>>>
requires vertex_property_map_for<Predecessor, G> &&
         vertex_property_map_for<Weight, G> &&
         basic_edge_weight_function<G, WF, vertex_property_map_value_t<Weight>, CompareOp,
                                    plus<vertex_property_map_value_t<Weight>>>
auto prim(G&&                   g,
          const vertex_id_t<G>& seed,
          Predecessor&          predecessor,
          Weight&               weight,
          WF&&                  weight_fn = [](const auto&, const edge_t<G>& uv) { return edge_value(g, uv); },
          CompareOp             compare   = less<vertex_property_map_value_t<Weight>>());
