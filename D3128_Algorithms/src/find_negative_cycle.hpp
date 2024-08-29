template <index_adjacency_list G, ranges::forward_range Predecessors, class OutputIterator>
requires output_iterator<OutputIterator, vertex_id_t<G>>
void find_negative_cycle(G&                              g,
                         const Predecessors&             predecessor,
                         const optional<vertex_id_t<G>>& cycle_vertex_id,
                         OutputIterator                  out_cycle);
