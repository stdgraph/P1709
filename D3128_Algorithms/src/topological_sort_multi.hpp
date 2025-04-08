template <index_adjacency_list G, input_range Sources, class Predecessors>
requires convertible_to<range_value_t<Sources>, vertex_id_t<G>> &&
         convertible_to<vertex_id_t<G>, range_value_t<Predecessors>>
void topological_sort(const G&       graph,
                      const Sources& sources,
                      Predecessors&  predecessors);
