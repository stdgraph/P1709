template <index_adjacency_list        G,
          ranges::random_access_range Distances,
          ranges::random_access_range Predecessors,
          class WF        = function<ranges::range_value_t<Distances>(edge_reference_t<G>)>
          class Allocator = allocator<vertex_id_t<G>>
          >
requires is_arithmetic_v<ranges::range_value_t<Distances>> &&
         convertible_to<vertex_id_t<G>, ranges::range_value_t<Predecessors>> &&
         edge_weight_function<G, WF, ranges::range_value_t<Distances>>
void bellman_ford_shortest_paths(
      G&&            g,            // graph
      vertex_id_t<G> source,       // starting vertex\_id
      Distances&     distances,    // out: Distances[uid] of uid from source
      Predecessors&  predecessors, // out: predecessor[uid] of uid in path
      WF&&           weight =
            [](edge_reference_t<G> uv) { return ranges::range_value_t<Distances>(1); },
      Allocator alloc = Allocator())                                                  

template <index_adjacency_list        G,
          ranges::random_access_range Distances,
          class WF        = std::function<ranges::range_value_t<Distances>(edge_reference_t<G>)>,
          class Allocator = allocator<vertex_id_t<G>>
          >
requires is_arithmetic_v<ranges::range_value_t<Distances>> &&
         edge_weight_function<G, WF, ranges::range_value_t<Distances>>
void bellman_ford_shortest_distances(
      G&&            g,         // graph
      vertex_id_t<G> seed,      // starting vertex\_id
      Distances&     distances, // out: Distances[uid] of uid from seed
      WF&&           weight =
            [](edge_reference_t<G> uv) { return ranges::range_value_t<Distances>(1); },
      Allocator alloc = Allocator());
