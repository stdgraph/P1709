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
          class          WeightFn,
          class          PredecessorFn,
          class WF      = function<distance_fn_value_t<WeightFn, G>(const remove_reference_t<G>&,
                                                                      const edge_t<G>&)>,
          class CompareOp = less<distance_fn_value_t<WeightFn, G>>>
requires distance_fn_for<WeightFn, G> &&
         is_arithmetic_v<distance_fn_value_t<WeightFn, G>> &&
         predecessor_fn_for<PredecessorFn, G> &&
         basic_edge_weight_function<G, WF, distance_fn_value_t<WeightFn, G>, CompareOp,
                                    plus<distance_fn_value_t<WeightFn, G>>>
auto prim(G&&                   g,
          const vertex_id_t<G>& seed,
          WeightFn&&            weight,
          PredecessorFn&&       predecessor,
          WF&&                  weight_fn = [](const auto& gr, const edge_t<G>& uv) { return edge_value(gr, uv); },
          CompareOp             compare   = less<distance_fn_value_t<WeightFn, G>>());
