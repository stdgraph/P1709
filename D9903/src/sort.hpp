template <index_adjacency_list G>
void sort_by_source(G&& graph);

template <index_adjacency_list G>
void sort_by_target(G&& graph);

template <index_adjacency_list G>
void sort_by_degree(G&& graph);

template <index_adjacency_list G, class W>
requires weight_function<W, edge_reference_t<G>>
void sort_by_weight(G&& graph,  W&& w);

template <edgelist::edgelist E>
void sort_by_source(E&& e);

template <edge_list_graph G>
void sort_by_target(E&& e);

template <edge_list_graph G>
void sort_by_degree(E&& e);

template <edge_list_graph G, class W>
void sort_by_weight(E&& e,  W&& w);