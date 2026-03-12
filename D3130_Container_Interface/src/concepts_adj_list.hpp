// For exposition only

template <class G>
concept adjacency_list = requires(G& g, vertex_t<G> u) {
  { vertices(g) } -> vertex_range<G>;
  { out_edges(g, u) } -> out_edge_range<G>;
};

template <class G>
concept index_adjacency_list = adjacency_list<G> &&
                               index_vertex_range<G>;

template <class G>
concept bidirectional_adjacency_list =
    adjacency_list<G> &&
    requires(G& g, vertex_t<G> u, in_edge_t<G> ie) {
      { in_edges(g, u) } -> in_edge_range<G>;
      { source_id(g, ie) } -> convertible_to<vertex_id_t<G>>;
    };

template <class G>
concept index_bidirectional_adjacency_list =
    bidirectional_adjacency_list<G> &&
    index_vertex_range<G>;

template <class G>
concept mapped_adjacency_list = adjacency_list<G> &&
                                mapped_vertex_range<G>;

template <class G>
concept mapped_bidirectional_adjacency_list =
    bidirectional_adjacency_list<G> &&
    mapped_vertex_range<G>;
