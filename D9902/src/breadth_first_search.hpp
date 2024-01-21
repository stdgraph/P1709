template <index_adjacency_list        G,
          ranges::random_access_range Distances,
          ranges::random_access_range Predecessors,
          class Allocator = allocator<vertex_id_t<G>>
          >
requires is_arithmetic_v<ranges::range_value_t<Distances>> &&
         convertible_to<vertex_id_t<G>, ranges::range_value_t<Predecessors>>
void breadth_first_search(
      G&&            g,            // graph
      vertex_id_t<G> source,       // starting vertex\_id
      Distances&     distances,    // out: Distances[uid] of uid from source in number of edges
      Predecessors&  predecessors, // out: predecessor[uid] of uid in path
      Allocator      alloc = Allocator());                                             

template <index_adjacency_list        G,
          ranges::random_access_range Distances,
          class Allocator = allocator<vertex_id_t<G>>
          >
requires is_arithmetic_v<ranges::range_value_t<Distances>>
void breadth_first_search(
      G&&            g,         // graph
      vertex_id_t<G> seed,      // starting vertex\_id
      Distances&     distances, // out: Distances[uid] of uid from seed in number of edges
      Allocator      alloc = Allocator());
