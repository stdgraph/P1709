template <class G>
concept basic_adjacency_list = vertex_range<G> &&              //
                               basic_targeted_edge_range<G> && //
                               targeted_edge<G, edge_t<G>>;

template <class G>
concept basic_index_adjacency_list = index_vertex_range<G> &&        //
                                     basic_targeted_edge_range<G> && //
                                     basic_targeted_edge<G, edge_t<G>>;
template <class G>
concept basic_sourced_adjacency_list = vertex_range<G> &&              //
                                       basic_targeted_edge_range<G> && //
                                       basic_sourced_targeted_edge<G, edge_t<G>>;

template <class G>
concept basic_sourced_index_adjacency_list = index_vertex_range<G> &&        //
                                             basic_targeted_edge_range<G> && //
                                             basic_sourced_targeted_edge<G, edge_t<G>>;
