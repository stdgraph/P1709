template <class G>
concept basic_adjacency_list = vertex_range<G> &&            //
                               basic_target_edge_range<G> && //
                               targeted_edge<G, edge_t<G>>;

template <class G>
concept basic_index_adjacency_list = index_vertex_range<G> &&      //
                                     basic_target_edge_range<G> && //
                                     targeted_edge<G, edge_t<G>>;

template <class G>
concept basic_sourced_adjacency_list = basic_adjacency_list<G> && //
                                       sourced_targeted_edge<G, edge_t<G>>;

template <class G>
concept basic_sourced_index_adjacency_list = basic_index_adjacency_list<G> && //
                                             sourced_targeted_edge<G, edge_t<G>>;
