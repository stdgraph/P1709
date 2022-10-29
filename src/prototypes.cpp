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
