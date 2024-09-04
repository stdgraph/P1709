template <adjacency_list G>
class dijkstra_visitor_base {
public:
  using graph_type             = G;
  using vertex_desc_type       = vertex_descriptor<vertex_id_t<G>, vertex_reference_t<G>, void>;
  using sourced_edge_desc_type = edge_descriptor<vertex_id_t<G>, true, edge_reference_t<G>, void>;

  // vertex visitor functions
  constexpr void on_initialize_vertex(const vertex_desc_type& vdesc) {}
  constexpr void on_discover_vertex(const vertex_desc_type& vdesc) {}
  constexpr void on_examine_vertex(const vertex_desc_type& vdesc) {}
  constexpr void on_finish_vertex(const vertex_desc_type& vdesc) {}

  // edge visitor functions
  constexpr void on_examine_edge(const sourced_edge_desc_type& edesc) {}
  constexpr void on_edge_relaxed(const sourced_edge_desc_type& edesc) {}
  constexpr void on_edge_not_relaxed(const sourced_edge_desc_type& edesc) {}
};
