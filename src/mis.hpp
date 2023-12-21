template <index_adjacency_list G, class Iter>
requires std::output_iterator<Iter, vertex_id_t<G>>
void maximal_independent_set(G&& g, Iter mis, vertex_id_t<G> seed) {}