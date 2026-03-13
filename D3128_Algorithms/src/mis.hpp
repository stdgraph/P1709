template <adjacency_list G, class Iter>
requires output_iterator<Iter, vertex_id_t<G>>
size_t maximal_independent_set(G&& g, Iter mis, const vertex_id_t<G>& seed = 0);
