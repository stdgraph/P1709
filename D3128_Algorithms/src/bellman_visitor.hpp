template <adjacency_list G>
class bellman_visitor_base {
public:
  using graph_type             = G;
  using sourced_edge_desc_type = edge_descriptor<vertex_id_t<G>, true, edge_reference_t<G>, void>;

  constexpr void on_examine_edge(sourced_edge_desc_type&& edesc) {}
  constexpr void on_edge_relaxed(sourced_edge_desc_type&& edesc) {}
  constexpr void on_edge_not_relaxed(sourced_edge_desc_type&& edesc) {}
  constexpr void on_edge_minimized(sourced_edge_desc_type&& edesc) {}
  constexpr void on_edge_not_minimized(sourced_edge_desc_type&& edesc) {}
};
