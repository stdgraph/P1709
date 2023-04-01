template <class DistanceValue>
auto dijkstra_invalid_distance() {
  return std::numeric_limits<DistanceValue>::max(); // exposition only
}

template <class DistanceValue>
auto dijkstra_zero() {
  return {}; // exposition only
}

template <property P, property D>
void init_dijkstra_shortest_paths(P&& predecessors, D&& distances);

template <property P, property D>
void init_dijkstra_shortest_paths(P&&                           predecessors,
                                  D&&                           distances,
                                  std::ranges::range_value_t<D> init);

template <property D>
void init_dijkstra_shortest_distances(D&& distances);

template <property D>
void init_dijkstra_shortest_distances(D&& distances, std::ranges::range_value_t<D> init);

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