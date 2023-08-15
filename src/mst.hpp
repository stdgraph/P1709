/*
 * Kruskal's Algorithm
 */
template <edgelist::edgelist E, edgelist::edgelist T>
void kruskal(E&& e, T&& t) {}

template <edgelist::edgelist E, edgelist::edgelist T, CompareOp>
void kruskal(E&& e, T&& t, CompareOp compare) {}

/*
 * Prim's Algorithm
 */
template <adjacency_list              G,
          ranges::random_access_range Predecessor,
          ranges::random_access_range Weight>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void prim(G&& g, Predecessor& predecessor, Weight& weight, vertex_id_t<G> seed = 0) {}

template <adjacency_list              G,
          ranges::random_access_range Predecessor,
          ranges::random_access_range Weight,
          class CompareOp>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void prim(G&&                           g,
          Predecessor&                  predecessor,
          Weight&                       weight,
          CompareOp                     compare,
          ranges::range_value_t<Weight> init_dist,
          vertex_id_t<G>                seed = 0) {}
