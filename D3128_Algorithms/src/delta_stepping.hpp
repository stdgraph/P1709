template <class ExecutionPolicy, adjacency_list_graph G, class W, property D, property P>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_paths(ExecutionPolicy&& e,
                                   const G&          graph,
                                   vertex_id_t<G>    source,
                                   W&&               w,
                                   D&&               distances,
                                   P&&               predecessors);

template <class ExecutionPolicy,
          adjacency_list_graph G,
          class W,
          property D,
          property P,
          class Compare,
          class Combine>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_paths(ExecutionPolicy&& e,
                                   const G&          graph,
                                   vertex_id_t<G>    source,
                                   W&&               w,
                                   D&&               distances,
                                   P&&               predecessors,
                                   Compare&&         comp,
                                   Combine&&         comb);

template <class ExecutionPolicy,
          adjacency_list_graph G,
          class W,
          property D,
          property P,
          class T>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_paths(ExecutionPolicy&& e,
                                   const G&          graph,
                                   vertex_id_t<G>    source,
                                   W&&               w,
                                   D&&               distances,
                                   P&&               predecessors,
                                   T                 delta);

template <class ExecutionPolicy,
          adjacency_list_graph G,
          class W,
          property D,
          property P,
          class Compare,
          class Combine,
          class T>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_paths(ExecutionPolicy&& e,
                                   const G&          graph,
                                   vertex_id_t<G>    source,
                                   W&&               w,
                                   D&&               distances,
                                   P&&               predecessors,
                                   Compare&&         comp,
                                   Combine&&         comb,
                                   T                 delta);

template <class ExecutionPolicy, adjacency_list_graph G, class W, property D>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_distances(
				       ExecutionPolicy&& e, const G& graph, vertex_id_t<G> source, W&& w, D&& distances);

template <class ExecutionPolicy,
          adjacency_list_graph G,
          class W,
          property D,
          class Compare,
          class Combine>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_distances(ExecutionPolicy&& e,
                                       const G&          graph,
                                       vertex_id_t<G>    source,
                                       W&&               w,
                                       D&&               distances,
                                       Compare&&         comp,
                                       Combine&&         comb);

template <class ExecutionPolicy, adjacency_list_graph G, class W, property D, class T>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_distances(ExecutionPolicy&& e,
                                       const G&          graph,
                                       vertex_id_t<G>    source,
                                       W&&               w,
                                       D&&               distances,
                                       T                 delta);

template <class ExecutionPolicy,
          adjacency_list_graph G,
          class W,
          property D,
          class Compare,
          class Combine,
          class T>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_distances(ExecutionPolicy&& e,
                                       const G&          graph,
                                       vertex_id_t<G>    source,
                                       W&&               w,
                                       D&&               distances,
                                       Compare&&         comp,
                                       Combine&&         comb,
                                       T                 delta);
