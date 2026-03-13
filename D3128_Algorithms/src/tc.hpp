/*
 * Triangle Counting Algorithm
 */
template <adjacency_list G>
requires ordered_vertex_edges<G>
[[nodiscard]] size_t triangle_count(G&& g) noexcept;
