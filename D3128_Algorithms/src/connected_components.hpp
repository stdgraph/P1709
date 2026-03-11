/*
 * Hopcroft-Tarjan Articulation Points
 */
template <index_adjacency_list G, class Iter>
requires output_iterator<Iter, vertex_id_t<G>>
void articulation_points(G&& g, Iter cut_vertices);

/*
 * Hopcroft-Tarjan Biconnected Components
 */
template <index_adjacency_list G, class OuterContainer>
void biconnected_components(G&& g, OuterContainer& components);

/*
 * Connected Components
 */
template <index_adjacency_list G, random_access_range Component>
size_t connected_components(G&& g, Component& component);

/*
 * Afforest Connected Components
 */
template <index_adjacency_list G, random_access_range Component>
void afforest(G&& g, Component& component, const size_t neighbor_rounds = 2);

template <index_adjacency_list G, adjacency_list GT, random_access_range Component>
void afforest(G&& g, GT&& g_t, Component& component, const size_t neighbor_rounds = 2);

/*
 * Kosaraju Strongly Connected Components
 */
template <index_adjacency_list G, index_adjacency_list GT, random_access_range Component>
void kosaraju(G&& g, GT&& g_t, Component& component);

template <index_bidirectional_adjacency_list G, random_access_range Component>
void kosaraju(G&& g, Component& component);
