/*
 * Kosaraju Strongly Connected Components
 */
template <adjacency_list G,
          adjacency_list GT,
          ranges::random_access_range Component>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>> &&
requires ranges::random_access_range<vertex_range_t<GT>> && integral<vertex_id_t<GT>>
void kosaraju(G&&        g,
              GT&&       g_t,
              Component& component) {}

/*
 * Tarjan Strongly Connected Components
 */
template <adjacency_list G,
          ranges::random_access_range Component>
requires ranges::random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>
void tarjan_cc(G&&        g,
               Component& component) {}
