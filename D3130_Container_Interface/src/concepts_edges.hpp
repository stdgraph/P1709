// For exposition only

template <class G>                                
concept targeted_edge = requires(G&& g, edge_reference_t<G> uv) { 
                            target(g, uv); 
                            target_id(g, uv);
                        };

template <class G>                              
concept sourced_edge = requires(G&& g, edge_reference_t<G> uv) { 
                        source(g, uv); 
                        source_id(g, uv);
                    };

template <class G>                                  
concept sourced_targeted_edge = targeted_edge<G> && sourced_edge<G>;
