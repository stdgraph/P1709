template <class G, class E>
concept targeted_edge = requires(G&& g, edge_reference_t<G> uv) {
  target_id(g, uv);
  target(g, uv);
};

template <class G, class E>
concept sourced_edge = requires(G&& g, edge_reference_t<G> uv) {
  source_id(g, uv);
  source(g, uv);
};

template <class G, class E>
concept sourced_targeted_edge = targeted_edge<G, E> && sourced_edge<G, E> && //
                                requires(G&& g, edge_reference_t<G> uv) { edge_id(g, uv); };
