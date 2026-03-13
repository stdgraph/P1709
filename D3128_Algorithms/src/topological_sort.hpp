// Single-source topological sort
template <adjacency_list G, class OutputIterator>
requires output_iterator<OutputIterator, vertex_id_t<G>>
[[nodiscard]] bool
topological_sort(const G& g, const vertex_id_t<G>& source, OutputIterator result);

// Full-graph topological sort
template <adjacency_list G, class OutputIterator>
requires output_iterator<OutputIterator, vertex_id_t<G>>
[[nodiscard]] bool topological_sort(const G& g, OutputIterator result);
