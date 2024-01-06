template <index_adjacency_list G, class Predecessors>
void breadth_first_search(const G&       graph,
                          vertex_id_t<G> source,
                          Predecessors&  predecessors);
