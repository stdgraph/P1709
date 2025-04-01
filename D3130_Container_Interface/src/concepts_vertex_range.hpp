// For exposition only

template <class G>                                                       // (exposition only)
concept _common_vertex_range = ranges::sized_range<vertex_range_t<G>> && //
                               requires(G&& g, vertex_iterator_t<G> ui) { vertex_id(g, u); };

template <class G>                                                
concept vertex_range = _common_vertex_range<vertex_range_t<G>> && //
                       ranges::forward_range<vertex_range_t<G>>;

template <class G>                                                             
concept index_vertex_range = _common_vertex_range<vertex_range_t<G>> &&        //
                             ranges::random_access_range<vertex_range_t<G>> && //
                             integral<vertex_id_t<G>>;
