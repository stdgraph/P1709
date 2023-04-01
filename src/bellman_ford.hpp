template <class ExecutionPolicy, edge_list_graph G, property D, property P>
void bellman_ford_shortest_paths(ExecutionPolicy&& e,
                                 const G&          graph,
                                 vertex_id_t<G>    source,
                                 P&&               predecessors,
                                 D&&               distances);
template <edge_list_graph G, class W, property D, property P, class Compare, class Combine>
requires weight_function<W, edge_t<G>>
void bellman_ford_shortest_paths(const G&       graph,
                                 vertex_id_t<G> source,
                                 W&&            w,
                                 D&&            distances,
                                 P&&            predecessors,
                                 Compare&&      comp,
                                 Combine&&      comb) {}

template <class ExecutionPolicy, edge_list_graph G, class W, property D, property P>
requires weight_function<W, edge_t<G>>
void bellman_ford_shortest_paths(ExecutionPolicy&& e,
                                 const G&          graph,
                                 vertex_id_t<G>    source,
                                 W&&               w,
                                 D&&               distances,
                                 P&&               predecessors) {}

template <class ExecutionPolicy,
          edge_list_graph G,
          class W,
          property D,
          property P,
          class Compare,
          class Combine>
requires weight_function<W, edge_t<G>>
void bellman_ford_shortest_paths(ExecutionPolicy&& e,
                                 const G&          graph,
                                 vertex_id_t<G>    source,
                                 W&&               w,
                                 D&&               distances,
                                 P&&               predecessors,
                                 Compare&&         comp,
                                 Combine&&         comb) {}

template <class ExecutionPolicy, edge_list_graph G, property D>
void bellman_ford_shortest_distances(ExecutionPolicy&& e,
                                     const G&          graph,
                                     vertex_id_t<G>    source,
                                     D&&               distances);
template <edge_list_graph G, class W, property D, class Compare, class Combine>
requires weight_function<W, edge_t<G>>
void bellman_ford_shortest_distances(const G&       graph,
                                     vertex_id_t<G> source,
                                     W&&            w,
                                     D&&            distances,
                                     Compare&&      comp,
                                     Combine&&      comb) {}

template <class ExecutionPolicy, edge_list_graph G, class W, property D>
requires weight_function<W, edge_t<G>>
void bellman_ford_shortest_distances(
      ExecutionPolicy&& e, const G& graph, vertex_id_t<G> source, W&& w, D&& distances) {}

template <class ExecutionPolicy,
          edge_list_graph G,
          class W,
          property D,
          class Compare,
          class Combine>
requires weight_function<W, edge_t<G>>
void bellman_ford_shortest_distances(ExecutionPolicy&& e,
                                     const G&          graph,
                                     vertex_id_t<G>    source,
                                     W&&               w,
                                     D&&               distances,
                                     Compare&&         comp,
                                     Combine&&         comb) {}