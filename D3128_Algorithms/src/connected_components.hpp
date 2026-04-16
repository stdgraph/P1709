/*
 * Hopcroft-Tarjan Articulation Points
 */
template <adjacency_list G, class Iter, class Alloc = allocator<byte>>
requires output_iterator<Iter, vertex_id_t<G>>
void articulation_points(G&& g, Iter cut_vertices, const Alloc& alloc = Alloc());

/*
 * Hopcroft-Tarjan Biconnected Components
 */
template <adjacency_list G, class OuterContainer, class Alloc = allocator<byte>>
void biconnected_components(G&& g, OuterContainer& components, const Alloc& alloc = Alloc());

/*
 * Connected Components
 */
template <adjacency_list G, class ComponentFn, class Alloc = allocator<byte>>
requires vertex_property_fn_for<ComponentFn, G>
size_t connected_components(G&& g, ComponentFn&& component, const Alloc& alloc = Alloc());

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
template <adjacency_list G, adjacency_list GT, class ComponentFn,
          class Alloc = allocator<byte>>
requires vertex_property_fn_for<ComponentFn, G>
void kosaraju(G&& g, GT&& g_t, ComponentFn&& component, const Alloc& alloc = Alloc());

template <bidirectional_adjacency_list G, class ComponentFn,
          class Alloc = allocator<byte>>
requires vertex_property_fn_for<ComponentFn, G>
void kosaraju(G&& g, ComponentFn&& component, const Alloc& alloc = Alloc());
