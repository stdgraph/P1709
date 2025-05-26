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
          random_access_range Predecessor,
          random_access_range Weight>
void prim(G&& g, Predecessor& predecessor, Weight& weight, vertex_id_t<G> source = 0);

template <index_adjacency_list        G,
          random_access_range Predecessor,
          random_access_range Weight,
          class CompareOp>
void prim(G&&                           g,
          Predecessor&                  predecessor,
          Weight&                       weight,
          CompareOp                     compare,
          range_value_t<Weight> init_dist,
          vertex_id_t<G>                source = 0);
