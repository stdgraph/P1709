/*
 * Hopcroft-Tarjan Articulation Points
 */
template <index_adjacency_list G, class Iter>
requires output_iterator<Iter, vertex_id_t<G>>
void articulation_points(G&&  g, Iter cut_vertices);

/*
 * Hopcroft-Tarjan Biconnected Components
 */
template <index_adjacency_list G,
          ranges::forward_range OuterContainer>
requires ranges::forward_range<ranges::range_value_t<OuterContainer>> &&
         integral<ranges::forward_range_t<ranges::forward_range_t<OuterContainer>>>
void biconnected_components(G&&             g,
			    OuterContainer& components);

/*
 * Connected Components
 */
template <index_adjacency_list G,
          ranges::random_access_range Component>
void connected_components(G&&        g,
			  Component& component);

/*
 * Kosaraju Strongly Connected Components
 */
template <index_adjacency_list G,
          index_adjacency_list GT,
          ranges::random_access_range Component>
void strongly_connected_components(G&&        g,
				   GT&&       g_t,
				   Component& component);

/*
 * Tarjan Strongly Connected Components
 */
template <adjacency_list G,
          ranges::random_access_range Component>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void strongly_connected_components(G&&        g,
	                          Component& component);
