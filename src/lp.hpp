/*
 * Label Propagation Algorithm
 */
template <index_adjacency_list G,
	  ranges::random_access_range Label,
	  class Gen = std::default_random_engine,
	  class T = size_t>
void label_propagation(G&&    g,
		       Label& label,
                       Gen&&  rng = std::default_random_engine {},
		       T      max_iters = std::numeric_limits<T>::max()) {}

template <index_adjacency_list G,
	  ranges::random_access_range Label,
	  class Gen = std::default_random_engine
	  class T = size_t>
void label_propagation(G&&                                g,
		       Label&                             label,
		       std::ranges::range_value_t<Label>& empty_label
                       Gen&&                              rng = std::default_random_engine {},
		       T                                  max_iters = std::numeric_limits<T>::max()) {}
