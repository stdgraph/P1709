/*
 * Kosaraju Strongly Connected Components
 */
template <adjacency_list G,
          adjacency_list GT,
          ranges::random_access_range Component>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>> &&
requires ranges::random_access_range<vertex_range_t<GT>> && integral<vertex_id_t<GT>>
void scc(G&&        g,
	 GT&&       g_t,
	 Component& component) {}

/*
 * Tarjan Strongly Connected Components
 */
template <adjacency_list G,
          ranges::random_access_range Component>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void scc(G&&        g,
	 Component& component) {}

/*
 * Hopcroft_Tarjan Articulation Points
 */
template <adjacency_list G,
          class Iter>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>> &&
         std::output_iterator<Iter, vertex_id_t<G>>
void articulation_points(G&&  g,
                         Iter cut_vertices) {}
/*
 * Hopcroft-Tarjan Biconnected Components
 */
template <adjacency_list G,
          class Container = std::set<vertex_id_t<G>>,
          class InnerIter = std::insert_iterator<Container>,
          class OuterIter >
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>> &&
         std::output_iterator<OuterIter, Container>
         && std::output_iterator<InnerIter, vertex_id_t<G>>
void bicc(G&&       g,
	  OuterIter component) {}

/*
 * Connected Components
 */
template <adjacency_list G,
          ranges::random_access_range Component>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void cc(G&&        g,
	Component& component) {}
