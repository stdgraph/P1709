/*
 * Kruskal's Algorithm
 */
template <x_index_edgelist_range IELR, x_index_edgelist_range OELR,
          class Alloc = allocator<byte>>
auto kruskal(IELR&& e, OELR&& t, const Alloc& alloc = Alloc());

template <x_index_edgelist_range IELR, x_index_edgelist_range OELR, class CompareOp,
          class Alloc = allocator<byte>>
auto kruskal(IELR&& e, OELR&& t, CompareOp compare, const Alloc& alloc = Alloc());

/*
 * Inplace Kruskal's Algorithm
 */
template <x_index_edgelist_range IELR, x_index_edgelist_range OELR,
          class Alloc = allocator<byte>>
requires permutable<iterator_t<IELR>>
auto inplace_kruskal(IELR&& e, OELR&& t, const Alloc& alloc = Alloc());

template <x_index_edgelist_range IELR, x_index_edgelist_range OELR, class CompareOp,
          class Alloc = allocator<byte>>
requires permutable<iterator_t<IELR>>
auto inplace_kruskal(IELR&& e, OELR&& t, CompareOp compare, const Alloc& alloc = Alloc());

/*
 * Prim's Algorithm
 */
template <adjacency_list G,
          class          WeightFn,
          class          PredecessorFn,
          class WF      = function<distance_fn_value_t<WeightFn, G>(const remove_reference_t<G>&,
                                                                      const edge_t<G>&)>,
          class CompareOp = less<distance_fn_value_t<WeightFn, G>>,
          class Heap    = use_default_heap,
          class Alloc   = allocator<byte>>
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
          CompareOp             compare   = less<distance_fn_value_t<WeightFn, G>>(),
          Heap                  heap_tag  = Heap{},
          const Alloc&          alloc     = Alloc());
