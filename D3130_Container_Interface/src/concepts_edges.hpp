template <class G, class E>
concept basic_targeted_edge = requires(G&& g, edge_reference_t<G> uv) { target_id(g, uv); };

template <class G, class E>
concept basic_sourced_edge = requires(G&& g, edge_reference_t<G> uv) { source_id(g, uv); };

template <class G, class E>
concept basic_sourced_targeted_edge = basic_targeted_edge<G, E> && //
                                      basic_sourced_edge<G, E> &&  //
                                      requires(G&& g, edge_reference_t<G> uv) { edge_id(g, uv); };
template <class G, class E>
concept targeted_edge = basic_targeted_edge<G, E> && //
                        requires(G&& g, edge_reference_t<G> uv) { target(g, uv); };

template <class G, class E>
concept sourced_edge = basic_sourced_edge<G, E> && //
                       requires(G&& g, edge_reference_t<G> uv) { source(g, uv); };

template <class G, class E>
concept sourced_targeted_edge = targeted_edge<G, E> && //
                                sourced_edge<G, E> &&  //
                                requires(G&& g, edge_reference_t<G> uv) { edge_id(g, uv); };
