template <index_adjacency_list G, class Predecessors>
convertible_to<vertex_id_t<G>, range_value_t<Predecessors>>
void topological_sort(const G&       graph,
                      vertex_id_t<G> source,
                      Predecessors&  predecessors);
