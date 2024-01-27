template <class G>
concept basic_target_edge_range = requires(G&& g, vertex_id_t<G> uid) {
  { edges(g, uid) } -> ranges::forward_range;
};

template <class G>
concept target_edge_range = basic_target_edge_range<G> && //
                            requires(G&& g, vertex_reference_t<G> u) {
                              { edges(g, u) } -> ranges::forward_range;
                            };
