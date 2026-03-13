/*
 * Hopcroft-Tarjan Articulation Points
 */
template <adjacency_list G, class Iter>
requires output_iterator<Iter, vertex_id_t<G>>
void articulation_points(G&& g, Iter cut_vertices);

/*
 * Hopcroft-Tarjan Biconnected Components
 */
template <adjacency_list G, class OuterContainer>
void biconnected_components(G&& g, OuterContainer& components);

/*
 * Connected Components
 */
template <adjacency_list G, class Component>
requires vertex_property_map_for<Component, G>
size_t connected_components(G&& g, Component& component);

/*
 * Afforest Connected Components
 */
template <adjacency_list G, class Component>
requires vertex_property_map_for<Component, G>
void afforest(G&& g, Component& component, const size_t neighbor_rounds = 2);

template <adjacency_list G, adjacency_list GT, class Component>
requires vertex_property_map_for<Component, G>
void afforest(G&& g, GT&& g_t, Component& component, const size_t neighbor_rounds = 2);

/*
 * Kosaraju Strongly Connected Components
 */
template <adjacency_list G, adjacency_list GT, class Component>
requires vertex_property_map_for<Component, G>
void kosaraju(G&& g, GT&& g_t, Component& component);

template <bidirectional_adjacency_list G, class Component>
requires vertex_property_map_for<Component, G>
void kosaraju(G&& g, Component& component);
