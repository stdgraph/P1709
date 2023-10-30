template <adjacency_list_graph G, property D, property P>
void breadth_first_search(const G&       graph,
                          vertex_id_t<G> source,
                          P&&            predecessors,
                          D&&            distances);

template <adjacency_list_graph G, property D, property P, queueable Q>
void breadth_first_search(
      const G& graph, vertex_id_t<G> source, P&& predecessors, D&& distances, Q&& q);

template <class ExecutionPolicy, adjacency_list_graph G, property D, property P>
void breadth_first_search(ExecutionPolicy&& policy,
                          const G&          graph,
                          vertex_id_t<G>    source,
                          P&&               predecessors,
                          D&&               distances);

template <class ExecutionPolicy, adjacency_list_graph G, property P, property D, queueable Q>
void breadth_first_search(ExecutionPolicy&& policy,
                          const G&          graph,
                          vertex_id_t<G>    source,
                          P&&               predecessors,
                          D&&               distances,
                          Q&&               q);

template <adjacency_list_graph G, property D>
void breadth_first_distances(const G& graph, vertex_id_t<G> source, D&& distances);

template <adjacency_list_graph G, property D, queueable Q>
void breadth_first_distances(const G& graph, vertex_id_t<G> source, D&& distances, Q&& q);

template <class ExecutionPolicy, adjacency_list_graph G, property D, property P>
void breadth_first_distances(ExecutionPolicy&& policy,
                             const G&          graph,
                             vertex_id_t<G>    source,
                             D&&               distances);

template <class ExecutionPolicy, adjacency_list_graph G, property D, queueable Q>
void breadth_first_distances(
      ExecutionPolicy&& policy, const G& graph, vertex_id_t<G> source, D&& distances, Q&& q);