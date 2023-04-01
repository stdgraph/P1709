template <adjacency_list G, class Iter>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>> &&
      std::output_iterator<Iter, vertex_id_t<G>>
void maximal_independent_set(G&& g, Iter mis, vertex_id_t<G> seed) {}