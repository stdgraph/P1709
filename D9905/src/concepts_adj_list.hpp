template <class G>
concept adjacency_list = vertex_range<G> &&        //
                         targeted_edge_range<G> && //
                         targeted_edge<G, edge_t<G>>;

template <class G>
concept index_adjacency_list = index_vertex_range<G> &&  //
                               targeted_edge_range<G> && //
                               targeted_edge<G, edge_t<G>>;
template <class G>
concept sourced_adjacency_list = vertex_range<G> &&        //
                                 targeted_edge_range<G> && //
                                 sourced_targeted_edge<G, edge_t<G>>;

template <class G>
concept sourced_index_adjacency_list = index_vertex_range<G> &&  //
                                       targeted_edge_range<G> && //
                                       sourced_targeted_edge<G, edge_t<G>>;
