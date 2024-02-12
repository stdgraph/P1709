template <index_adjacency_list G, 
          class Predecessors>
void topological_sort(const G&       graph,
                      vertex_id_t<G> source,
                      Predecessors&  predecessors);
