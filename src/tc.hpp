/*
 * Triangle Counting Algorithm
 */
template <adjacency_list G>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
size_t triangle_count(G&& g) {}
