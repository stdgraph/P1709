/**
 * @file   prototypes.cpp
 *
 * @section LICENSE
 *
 * The MIT License
 *
 * @section DESCRIPTION
 *
 * This file contains the prototypes given in [DP]1709, to at least make sure they will all compile correctly.
 */

/*
 * Potential algorithms:
 *
 *   betweenness_centrality
 *   breadth_first_search
 *   boykov_kolmogorov_max_flow
 *   connected_components
 *   delta_stepping
 *   dijkstra_shortest_path
 *   jaccard_similarity
 *   jones_plassmann_coloring
 *   k_core
 *   k_truss
 *   kruskal_minimum_spanning_forest
 *   push_relabel_max_flow
 *   maximal_independent_set (regular and dag-based)
 *   page_rank
 *   prim_minimum_spanning_tree
 *   spmatspmat
 *   triangle_count
 */


/*
 * Organization of algorithms
 *
 *   Shortest paths, basic interface
 *     shortest_paths
 *     shortest_distances
 *
 *     
 *   Shortest paths, advanced interface
 *     breadth_first_searhc
 *       single source
 *         paths and distances
 *         paths
 *       multi source
 *         paths and distances
 *         paths
 *     dijkstra (and s_t)
 *       single source
 *         paths and distances
 *         paths
 *       multi source
 *         paths and distances
 *         paths
 *     bellman_ford
 *     all_pairs
 *     a_star
 *
 *  Centrality
 *    Betweenness
 *
 *  Clustering
 *    triangle_counting
 *
 *  Coloring
 *    jones_plassman
 *
 *  Components
 *    connected
 *    strongly_connected
 *
 *  Connectivity
 *    minimum_cuts
 *
 *  Cores
 *    k_core
 *    k_truss
 *
 *  Dag
 *    topological_sort
 *    transitive_closure
 *
 *  Flows
 *    edmunds_karp
 *    push_relabel
 *    boykov_kolmogorov
 *
 *  Link Analysis
 *    page_rank
 *    jaccard_coefficient
 *
 *  Maximal Independent Set
 *    maximal_independent_set
 *
 *  Operators
 *    transpose
 *    join
 *
 *  Traversal
 *    breadth_first
 *    depth_first
 *
 */


#include <concepts>
#include <execution>
#include <list>
#include <queue>
#include <vector>
#include <ranges>

#include "print_types.hpp"

/*
 * Some pseudo-concepts
 */
template <class G>
concept adjacency_list_graph = std::copyable<G> && requires(G g) {
  g.out_edges();
};

template <class G>
concept bidirectional_adjacency_list_graph = std::copyable<G> && requires(G g) {
  g.out_edges();
  g.in_edges();
};

template <class G>
concept edge_list_graph = std::copyable<G>;

template <class P>
concept property = std::random_access_range<P>;

template <class Q>
concept queueable = std::copyable<Q> && requires(Q q) {
  q.pop();
};

template <class W, class E>
concept weight_function = requires(W w, E e) {
  w(e);
};


template <class G>
struct graph_traits {
  using vertex_id_type = size_t;
  using edge_type      = size_t;
};

template <class G>
using vertex_id_t = graph_traits<G>::vertex_id_type;

template <class G>
using edge_t = graph_traits<G>::edge_type;

#include "alg_synopsis.hpp"

struct adj {
  void out_edges() {}
};

struct bi_adj {
  void out_edges() {}
  void in_edges() {}
};

int main() {
  adj graph;
  bi_adj bi_graph;
  size_t source;
  size_t target;
  auto   w    = [](size_t) -> size_t { return 0UL; };
  auto   comp = [](size_t a, size_t b) -> size_t { return a < b; };
  auto   comb = [](size_t a, size_t b) -> size_t { return a + b; };

  shortest_paths(graph, source, std::vector<size_t>{}, std::vector<size_t>{});
  shortest_paths(bi_graph, source,std::vector<size_t>{},
                 std::vector<size_t>{});

  shortest_paths(std::execution::par, graph, source, std::vector<size_t>{},
                 std::vector<size_t>{});
  shortest_paths(std::execution::par, bi_graph, source,
                 std::vector<size_t>{}, std::vector<size_t>{});

  breadth_first_search(graph, source, std::vector<size_t>{},
                       std::vector<size_t>{});

  breadth_first_search(bi_graph, source,  std::vector<size_t>{},
                       std::vector<size_t>{});

  breadth_first_search(std::execution::par, graph, source,
                       std::vector<size_t>{}, std::vector<size_t>{});
  breadth_first_search(std::execution::par, bi_graph, source, 
                       std::vector<size_t>{}, std::vector<size_t>{});

  breadth_first_search(graph, source, std::vector<size_t>{},
                       std::vector<size_t>{}, std::queue<size_t>{});
  breadth_first_search(bi_graph, source,std::vector<size_t>{},
                       std::vector<size_t>{}, std::queue<size_t>{});

  breadth_first_search(std::execution::par, graph, source,
                       std::vector<size_t>{}, std::vector<size_t>{},
                       std::queue<size_t>{});
  breadth_first_search(std::execution::par, bi_graph, source,
                       std::vector<size_t>{}, std::vector<size_t>{},
                       std::queue<size_t>{});

  dijkstra_shortest_paths(graph, source, w, std::vector<size_t>{},
                          std::vector<size_t>{});
  dijkstra_shortest_paths(bi_graph, source, w, std::vector<size_t>{},
                          std::vector<size_t>{});

  dijkstra_shortest_paths(graph, source, w, std::vector<size_t>{},
                          std::vector<size_t>{}, comp, comb);
  dijkstra_shortest_paths(bi_graph, source, w, std::vector<size_t>{},
                          std::vector<size_t>{}, comp, comb);
}
