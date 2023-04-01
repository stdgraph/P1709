template <property P, property D>
void init_breadth_first_search(P&& predecessors, D&& distances);

template <property P, property D>
void init_breadth_first_search(P&&                           predecessors,
                               D&&                           distances,
                               std::ranges::range_value_t<D> init);

template <property D>
void init_breadth_first_distances(D&& distances);

template <property D>
void init_breadth_first_distances(D&& distances, std::ranges::range_value_t<D> init);

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