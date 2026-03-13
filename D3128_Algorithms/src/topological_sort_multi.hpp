template <adjacency_list G, input_range Sources, class OutputIterator>
requires convertible_to<range_value_t<Sources>, vertex_id_t<G>> &&
         output_iterator<OutputIterator, vertex_id_t<G>>
[[nodiscard]] bool topological_sort(const G& g, const Sources& sources, OutputIterator result);
