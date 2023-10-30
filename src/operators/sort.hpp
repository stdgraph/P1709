// TODO: requires sourced_adjacency_list?
template <class ExecutionPolicy, 
          adjacency_list_graph G>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void sort_by_source(ExecutionPolicy&& policy, G& graph) {}

template <class ExecutionPolicy, 
          adjacency_list_graph G>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void sort_by_target(ExecutionPolicy&& policy, G& graph) {}

template <class ExecutionPolicy, 
          adjacency_list_graph G>
void sort_by_degree(ExecutionPolicy&& policy, G& graph) {}

template <class ExecutionPolicy, 
          adjacency_list_graph G,
          class W>
requires weight_function<W, edge_t<G>> && ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void sort_by_weight(ExecutionPolicy&& policy, G& graph,  W&& w) {}

// Needs the same functionality for edge_list_graph (requires edgelist_range_t?)
template <class ExecutionPolicy, 
          edge_list_graph G>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void sort_by_source(ExecutionPolicy&& policy, G& graph) {}

template <class ExecutionPolicy, 
          edge_list_graph G>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void sort_by_target(ExecutionPolicy&& policy, G& graph) {}

template <class ExecutionPolicy, 
          edge_list_graph G>
void sort_by_degree(ExecutionPolicy&& policy, G& graph) {}

template <class ExecutionPolicy, 
          edge_list_graph G,
          class W>
requires weight_function<W, edge_t<G>> && ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void sort_by_weight(ExecutionPolicy&& policy, G& graph,  W&& w) {}