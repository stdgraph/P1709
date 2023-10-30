// TODO: requires sourced_adjacency_list?
template <adjacency_list_graph G>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void sort_by_source(G& graph) {}

template <adjacency_list_graph G>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void sort_by_target(G& graph) {}

template <adjacency_list_graph G>
void sort_by_degree(G& graph) {}

template <adjacency_list_graph G,
          class W>
requires weight_function<W, edge_t<G>> && ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void sort_by_weight(G& graph,  W&& w) {}

// Needs the same functionality for edge_list_graph (requires edgelist_range_t?)
template <edge_list_graph G>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void sort_by_source(G& graph) {}

template <edge_list_graph G>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void sort_by_target(G& graph) {}

template <edge_list_graph G>
void sort_by_degree(G& graph) {}

template <edge_list_graph G,
          class W>
requires weight_function<W, edge_t<G>> && ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void sort_by_weight(G& graph,  W&& w) {}