/*
 * Tarjan's Strongly Connected Components
 */
template <adjacency_list G, class ComponentFn>
requires vertex_property_fn_for<ComponentFn, G>
size_t tarjan_scc(G&& g, ComponentFn&& component);
