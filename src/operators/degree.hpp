template <adjacency_list_graph G, 
          ranges::random_access_range D>
requires is_integral_v<ranges::range_value_t<P>>
void degrees(const G& graph, D& degrees_per_vertex) {}