/*
 * Jaccard Coefficient Algorithm
 */
template <adjacency_list G, typename OutOp, typename T = double>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>> && is_invocable_v<OutOp, vertex_id_t<G>&, vertex_id_t<G>&, edge_t<G>&, T>
void jaccard_coefficient(G&& g, OutOp out) {}
