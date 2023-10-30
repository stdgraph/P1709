template <adjacency_list_graph G, class W, property D, property P>
requires weight_function<W, edge_t<G>>
void dijkstra_shortest_paths(
      const G& graph, vertex_id_t<G> source, W&& w, D&& distances, P&& predecessors) {}

template <adjacency_list_graph G, class W, property P, property D, class Compare, class Combine>
requires weight_function<W, edge_t<G>> &&
      std::strict_weak_order<Compare, typename D::value_type, typename D::value_type> &&
      std::assignable_from < typename D::reference,
      std::invoke_result_t < Combine, std::invoke_result_t<W, edge_t<G>>,
typename D::value_type >> void dijkstra_shortest_paths(const G&       graph,
                                                       vertex_id_t<G> source,
                                                       W&&            w,
                                                       D&&            distances,
                                                       P&&            predecessors,
                                                       Compare&&      comp,
                                                       Combine&&      comb) {}

template <adjacency_list_graph G, class W, property D>
requires weight_function<W, edge_t<G>>
void dijkstra_shortest_distances(const G& graph, vertex_id_t<G> source, W&& w, D&& distances) {}

template <adjacency_list_graph G, class W, property D, class Compare, class Combine>
requires weight_function<W, edge_t<G>> &&
      std::strict_weak_order<Compare, typename D::value_type, typename D::value_type> &&
      std::assignable_from < typename D::reference,
      std::invoke_result_t < Combine, std::invoke_result_t<W, edge_t<G>>,
typename D::value_type >> void dijkstra_shortest_distances(const G&       graph,
                                                           vertex_id_t<G> source,
                                                           W&&            w,
                                                           D&&            distances,
                                                           Compare&&      comp,
                                                           Combine&&      comb) {}