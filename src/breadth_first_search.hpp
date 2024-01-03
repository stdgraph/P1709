template <index_adjacency_list G, property D, property P>
void breadth_first_search(const G&       graph,
                          vertex_id_t<G> source,
                          P&&            predecessors,
                          D&&            distances);

template <index_adjacency_list G, property D, property P, queueable Q>
void breadth_first_search(
      const G& graph, vertex_id_t<G> source, P&& predecessors, D&& distances, Q&& q);

template <index_adjacency_list G, property D>
void breadth_first_distances(const G& graph, vertex_id_t<G> source, D&& distances);

template <index_adjacency_list G, property D, queueable Q>
void breadth_first_distances(const G& graph, vertex_id_t<G> source, D&& distances, Q&& q);
