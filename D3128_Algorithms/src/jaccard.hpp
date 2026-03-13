/*
 * Jaccard Coefficient Algorithm
 */
template <adjacency_list G, typename OutOp, typename T = double>
requires invocable<OutOp, vertex_id_t<G>, vertex_id_t<G>, edge_t<G>&, T>
void jaccard_coefficient(G&& g, OutOp out);
