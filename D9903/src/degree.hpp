template <index_adjacency_list G, 
          ranges::random_access_range D>
requires is_integral_v<ranges::range_value_t<D>>
void degrees(const G& graph, D& degrees_per_vertex);