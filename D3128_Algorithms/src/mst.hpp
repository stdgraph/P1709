/*
 * Kruskal's Algorithm
 */
template <index_edgelist_range IELR, index_edgelist_range OELR>
void kruskal(IELR&& e, OELR&& t);

template <index_edgelist_range IELR, index_edgelist_range OELR T, CompareOp>
void kruskal(IELR&& e, OELR&& t, CompareOp compare);

/*
 * Prim's Algorithm
 */
template <index_adjacency_list        G,
          ranges::random_access_range Predecessor,
          ranges::random_access_range Weight>
void prim(G&& g, Predecessor& predecessor, Weight& weight, vertex_id_t<G> seed = 0);

template <index_adjacency_list        G,
          ranges::random_access_range Predecessor,
          ranges::random_access_range Weight,
          class CompareOp>
void prim(G&&                           g,
          Predecessor&                  predecessor,
          Weight&                       weight,
          CompareOp                     compare,
          ranges::range_value_t<Weight> init_dist,
          vertex_id_t<G>                seed = 0);
