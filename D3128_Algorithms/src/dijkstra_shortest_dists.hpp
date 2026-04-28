template <adjacency_list G,
          class          DistanceFn,
          class WF      = function<distance_fn_value_t<DistanceFn, G>(const remove_reference_t<G>&,
                                                                       const edge_t<G>&)>,
          class Visitor = empty_visitor,
          class Compare = less<distance_fn_value_t<DistanceFn, G>>,
          class Combine = plus<distance_fn_value_t<DistanceFn, G>>,
          class Heap    = use_default_heap,
          class Alloc   = allocator<byte>>
requires distance_fn_for<DistanceFn, G> &&
         basic_edge_weight_function<G, WF, distance_fn_value_t<DistanceFn, G>, Compare, Combine>
constexpr void dijkstra_shortest_distances(
      G&&                   g,
      const vertex_id_t<G>& source,
      DistanceFn&&          distance,
      WF&&                  weight  = [](const auto&,
                       const edge_t<G>& uv) { return distance_fn_value_t<DistanceFn, G>(1); },
      Visitor&&             visitor = empty_visitor(),
      Compare&&             compare = less<distance_fn_value_t<DistanceFn, G>>(),
      Combine&&             combine = plus<distance_fn_value_t<DistanceFn, G>>(),
      Heap                  heap_tag = Heap{},
      const Alloc&          alloc   = Alloc());
