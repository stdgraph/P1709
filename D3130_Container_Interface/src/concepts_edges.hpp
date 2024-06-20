// For exposition only

template <class G> 
concept basic_targeted_edge = requires(G&& g, edge_reference_t<G> uv) { target_id(g, uv); };

template <class G> 
concept basic_sourced_edge = requires(G&& g, edge_reference_t<G> uv) { source_id(g, uv); };

template <class G>                                              
concept basic_sourced_targeted_edge = basic_targeted_edge<G> && //
                                      basic_sourced_edge<G> &&  //
                                      requires(G&& g, edge_reference_t<G> uv) { edge_id(g, uv); };

template <class G>                                
concept targeted_edge = basic_targeted_edge<G> && //
                        requires(G&& g, edge_reference_t<G> uv) { target(g, uv); };

template <class G>                              
concept sourced_edge = basic_sourced_edge<G> && //
                       requires(G&& g, edge_reference_t<G> uv) { source(g, uv); };

template <class G>                                  
concept sourced_targeted_edge = targeted_edge<G> && //
                                sourced_edge<G> &&  //
                                requires(G&& g, edge_reference_t<G> uv) { edge_id(g, uv); };
