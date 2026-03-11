/*
 * Triangle Counting Algorithm
 */
template <index_adjacency_list G>
requires ordered_vertex_edges<G>
[[nodiscard]] size_t triangle_count(G&& g) noexcept;
