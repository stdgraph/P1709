template <adjacency_list_graph G, property D, property P>
requires(!bidirectional_adjacency_list_graph<G>) 
void shortest_paths(const G&       graph,
                    vertex_id_t<G> source,
                    D&&            distances,
                    P&&            predecessors);

template <bidirectional_adjacency_list_graph G, property D, property P>
void shortest_paths(const G&       graph, 
                    vertex_id_t<G> source, 
                    D&&            distances, 
                    P&&            predecessors);

template <adjacency_list_graph G, property D, property P>
requires(!bidirectional_adjacency_list_graph<G>) 
void shortest_paths(const G&          graph,
                    vertex_id_t<G>    source,
                    D&&               distances,
                    P&&               predecessors);

template <bidirectional_adjacency_list_graph G, property D, property P>
void shortest_paths(const G&          graph,
                    vertex_id_t<G>    source,
                    D&&               distances,
                    P&&               predecessors);

template <adjacency_list_graph G, property D>
requires(!bidirectional_adjacency_list_graph<G>) 
void shortest_distances(const G&       graph,
                        vertex_id_t<G> source,
                        D&&            distances);

template <bidirectional_adjacency_list_graph G, property D>
void shortest_distances(const G&       graph, 
                        vertex_id_t<G> source, 
                        D&&            distances);

template <adjacency_list_graph G, property D>
requires(!bidirectional_adjacency_list_graph<G>) 
void shortest_distances(const G&       graph, 
                        vertex_id_t<G> source, 
                        D&&            distances);

template <bidirectional_adjacency_list_graph G, property D>
void shortest_distances(const G&          graph,
                        vertex_id_t<G>    source,
                        D&&               distances);
