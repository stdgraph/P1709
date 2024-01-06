template <index_adjacency_list G, class Predecessors, class Allocator = allocator<vertex_id_t<G>>
void breadth_first_search(const G&       graph,
                          vertex_id_t<G> source,
                          Predecessors&  predecessors,
                          Allocator      alloc = Allocator());
