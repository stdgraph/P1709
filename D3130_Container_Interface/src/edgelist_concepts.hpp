template <class EL> // For exposition only
concept sourced_edgelist = ranges::input_range<EL> && 
                              !ranges::range<ranges::range_value_t<EL>> &&
                              requires(ranges::range_value_t<EL> e) {
                                { source_id(e) };
                                { target_id(e) } -> same_as<decltype(source_id(e))>;
                              };

template <class EL> // For exposition only
concept sourced_index_edgelist = sourced_edgelist<EL> &&
                                    requires(ranges::range_value_t<EL> e) {
                                      { source_id(e) } -> integral;
                                      { target_id(e) } -> integral; // redundant, for clarity
                                    };

template <class EL> // For exposition only
concept has_edge_value = sourced_edgelist<EL> && //
                            requires(ranges::range_value_t<EL> e) {
                              { edge_value(e) };
                            };
