/*
* Driver Functions
*/
template <adjacency_list_graph G, property D, property P>
requires(!bidirectional_adjacency_list_graph<G>) void shortest_paths(const G&       graph,
                                                                     vertex_id_t<G> source,
                                                                     D&&            distances,
                                                                     P&& predecessors);

template <bidirectional_adjacency_list_graph G, property D, property P>
void shortest_paths(const G& graph, vertex_id_t<G> source, D&& distances, P&& predecessors);

template <class ExecutionPolicy, adjacency_list_graph G, property D, property P>
requires(!bidirectional_adjacency_list_graph<G>) void shortest_paths(ExecutionPolicy&& policy,
                                                                     const G&          graph,
                                                                     vertex_id_t<G>    source,
                                                                     D&& distances,
                                                                     P&& predecessors);

template <class ExecutionPolicy, bidirectional_adjacency_list_graph G, property D, property P>
void shortest_paths(ExecutionPolicy&& policy,
                    const G&          graph,
                    vertex_id_t<G>    source,
                    D&&               distances,
                    P&&               predecessors);

template <adjacency_list_graph G, property D>
requires(!bidirectional_adjacency_list_graph<G>) void shortest_distances(const G&       graph,
                                                                         vertex_id_t<G> source,
                                                                         D&& distances);

template <bidirectional_adjacency_list_graph G, property D>
void shortest_distances(const G& graph, vertex_id_t<G> source, D&& distances);

template <class ExecutionPolicy, adjacency_list_graph G, property D>
requires(!bidirectional_adjacency_list_graph<G>) void shortest_distances(
      ExecutionPolicy&& policy, const G& graph, vertex_id_t<G> source, D&& distances);

template <class ExecutionPolicy, bidirectional_adjacency_list_graph G, property D>
void shortest_distances(ExecutionPolicy&& policy,
                        const G&          graph,
                        vertex_id_t<G>    source,
                        D&&               distances);

/*
 * Initialize BFS
 */
template <property P, property D>
void init_breadth_first_search(P&& predecessors, D&& distances);

template <property P, property D>
void init_breadth_first_search(P&&                           predecessors,
                               D&&                           distances,
                               std::ranges::range_value_t<D> init);

template <property D>
void init_breadth_first_distances(D&& distances);

template <property D>
void init_breadth_first_distances(D&& distances, std::ranges::range_value_t<D> init);

/*
 * BFS: single source
 */
template <adjacency_list_graph G, property D, property P>
void breadth_first_search(const G&       graph,
                          vertex_id_t<G> source,
                          P&&            predecessors,
                          D&&            distances);

template <adjacency_list_graph G, property D, property P, queueable Q>
void breadth_first_search(
      const G& graph, vertex_id_t<G> source, P&& predecessors, D&& distances, Q&& q);

template <class ExecutionPolicy, adjacency_list_graph G, property D, property P>
void breadth_first_search(ExecutionPolicy&& policy,
                          const G&          graph,
                          vertex_id_t<G>    source,
                          P&&               predecessors,
                          D&&               distances);

template <class ExecutionPolicy, adjacency_list_graph G, property P, property D, queueable Q>
void breadth_first_search(ExecutionPolicy&& policy,
                          const G&          graph,
                          vertex_id_t<G>    source,
                          P&&               predecessors,
                          D&&               distances,
                          Q&&               q);

template <adjacency_list_graph G, property D>
void breadth_first_distances(const G& graph, vertex_id_t<G> source, D&& distances);

template <adjacency_list_graph G, property D, queueable Q>
void breadth_first_distances(const G& graph, vertex_id_t<G> source, D&& distances, Q&& q);

template <class ExecutionPolicy, adjacency_list_graph G, property D, property P>
void breadth_first_distances(ExecutionPolicy&& policy,
                             const G&          graph,
                             vertex_id_t<G>    source,
                             D&&               distances);

template <class ExecutionPolicy, adjacency_list_graph G, property D, queueable Q>
void breadth_first_distances(
      ExecutionPolicy&& policy, const G& graph, vertex_id_t<G> source, D&& distances, Q&& q);

/*
 * Initialize Dijkstra
 */
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

/*
 * Dijkstra: single source
 */
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

/*
 * Bellman-Ford: single source
 */
template <class ExecutionPolicy, edge_list_graph G, property D, property P>
void bellman_ford_shortest_paths(ExecutionPolicy&& e,
                                 const G&          graph,
                                 vertex_id_t<G>    source,
                                 P&&               predecessors,
                                 D&&               distances);
template <edge_list_graph G, class W, property D, property P, class Compare, class Combine>
requires weight_function<W, edge_t<G>>
void bellman_ford_shortest_paths(const G&       graph,
                                 vertex_id_t<G> source,
                                 W&&            w,
                                 D&&            distances,
                                 P&&            predecessors,
                                 Compare&&      comp,
                                 Combine&&      comb) {}

template <class ExecutionPolicy, edge_list_graph G, class W, property D, property P>
requires weight_function<W, edge_t<G>>
void bellman_ford_shortest_paths(ExecutionPolicy&& e,
                                 const G&          graph,
                                 vertex_id_t<G>    source,
                                 W&&               w,
                                 D&&               distances,
                                 P&&               predecessors) {}

template <class ExecutionPolicy,
          edge_list_graph G,
          class W,
          property D,
          property P,
          class Compare,
          class Combine>
requires weight_function<W, edge_t<G>>
void bellman_ford_shortest_paths(ExecutionPolicy&& e,
                                 const G&          graph,
                                 vertex_id_t<G>    source,
                                 W&&               w,
                                 D&&               distances,
                                 P&&               predecessors,
                                 Compare&&         comp,
                                 Combine&&         comb) {}

template <class ExecutionPolicy, edge_list_graph G, property D>
void bellman_ford_shortest_distances(ExecutionPolicy&& e,
                                     const G&          graph,
                                     vertex_id_t<G>    source,
                                     D&&               distances);
template <edge_list_graph G, class W, property D, class Compare, class Combine>
requires weight_function<W, edge_t<G>>
void bellman_ford_shortest_distances(const G&       graph,
                                     vertex_id_t<G> source,
                                     W&&            w,
                                     D&&            distances,
                                     Compare&&      comp,
                                     Combine&&      comb) {}

template <class ExecutionPolicy, edge_list_graph G, class W, property D>
requires weight_function<W, edge_t<G>>
void bellman_ford_shortest_distances(
      ExecutionPolicy&& e, const G& graph, vertex_id_t<G> source, W&& w, D&& distances) {}

template <class ExecutionPolicy,
          edge_list_graph G,
          class W,
          property D,
          class Compare,
          class Combine>
requires weight_function<W, edge_t<G>>
void bellman_ford_shortest_distances(ExecutionPolicy&& e,
                                     const G&          graph,
                                     vertex_id_t<G>    source,
                                     W&&               w,
                                     D&&               distances,
                                     Compare&&         comp,
                                     Combine&&         comb) {}

/*
 * Delta stepping: single source
 */
template <class ExecutionPolicy, adjacency_list_graph G, class W, property D, property P>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_paths(ExecutionPolicy&& e,
                                   const G&          graph,
                                   vertex_id_t<G>    source,
                                   W&&               w,
                                   D&&               distances,
                                   P&&               predecessors) {}

template <class ExecutionPolicy,
          adjacency_list_graph G,
          class W,
          property D,
          property P,
          class Compare,
          class Combine>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_paths(ExecutionPolicy&& e,
                                   const G&          graph,
                                   vertex_id_t<G>    source,
                                   W&&               w,
                                   D&&               distances,
                                   P&&               predecessors,
                                   Compare&&         comp,
                                   Combine&&         comb) {}

template <class ExecutionPolicy,
          adjacency_list_graph G,
          class W,
          property D,
          property P,
          class T>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_paths(ExecutionPolicy&& e,
                                   const G&          graph,
                                   vertex_id_t<G>    source,
                                   W&&               w,
                                   D&&               distances,
                                   P&&               predecessors,
                                   T                 delta) {}

template <class ExecutionPolicy,
          adjacency_list_graph G,
          class W,
          property D,
          property P,
          class Compare,
          class Combine,
          class T>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_paths(ExecutionPolicy&& e,
                                   const G&          graph,
                                   vertex_id_t<G>    source,
                                   W&&               w,
                                   D&&               distances,
                                   P&&               predecessors,
                                   Compare&&         comp,
                                   Combine&&         comb,
                                   T                 delta) {}

template <class ExecutionPolicy, adjacency_list_graph G, class W, property D>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_distances(
      ExecutionPolicy&& e, const G& graph, vertex_id_t<G> source, W&& w, D&& distances) {}

template <class ExecutionPolicy,
          adjacency_list_graph G,
          class W,
          property D,
          class Compare,
          class Combine>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_distances(ExecutionPolicy&& e,
                                       const G&          graph,
                                       vertex_id_t<G>    source,
                                       W&&               w,
                                       D&&               distances,
                                       Compare&&         comp,
                                       Combine&&         comb) {}

template <class ExecutionPolicy, adjacency_list_graph G, class W, property D, class T>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_distances(ExecutionPolicy&& e,
                                       const G&          graph,
                                       vertex_id_t<G>    source,
                                       W&&               w,
                                       D&&               distances,
                                       T                 delta) {}

template <class ExecutionPolicy,
          adjacency_list_graph G,
          class W,
          property D,
          class Compare,
          class Combine,
          class T>
requires weight_function<W, edge_t<G>>
void delta_stepping_shortest_distances(ExecutionPolicy&& e,
                                       const G&          graph,
                                       vertex_id_t<G>    source,
                                       W&&               w,
                                       D&&               distances,
                                       Compare&&         comp,
                                       Combine&&         comb,
                                       T                 delta) {}

/*
 * Maximal Independent Set
 */
template <adjacency_list G, class Iter>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>> &&
      std::output_iterator<Iter, vertex_id_t<G>>
void maximal_independent_set(G&& g, Iter mis, vertex_id_t<G> seed) {}

/* 
 * Page Rank
 */
template <class ExecutionPolicy,
          adjacency_list_graph        G,
          ranges::random_access_range P,
          class EVF = std::function<ranges::range_value_t<P>(edge_reference_t<G>)>>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>> &&
      is_arithmetic_v<ranges::range_value_t<P>> && edge_weight_function<G, EVF>
void pagerank(ExecutionPolicy   policy,
              G&&               g,
              P&                scores,
              const double      damping_factor,
              const double      threshold,
              const std::size_t max_iterations,
              EVF               weight_fn) {}

template <adjacency_list_graph        G,
          ranges::random_access_range P,
          class EVF = std::function<ranges::range_value_t<P>(edge_reference_t<G>)>>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>> &&
      is_arithmetic_v<ranges::range_value_t<P>> && edge_weight_function<G, EVF>
void pagerank(G&&               g,
              P&                scores,
              const double      damping_factor,
              const double      threshold,
              const std::size_t max_iterations,
              EVF               weight_fn) {}
