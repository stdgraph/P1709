/*
 * Label Propagation Algorithm
 */
template <index_adjacency_list G,
	  random_access_range Label,
	  class Gen = default_random_engine,
	  class T = size_t>
void label_propagation(G&&    g,
		       Label& label,
                       Gen&&  rng = default_random_engine {},
		       T      max_iters = numeric_limits<T>::max());

template <index_adjacency_list G,
	  random_access_range Label,
	  class Gen = default_random_engine
	  class T = size_t>
void label_propagation(G&&                                g,
		       Label&                             label,
		       range_value_t<Label>& empty_label
                       Gen&&                              rng = default_random_engine {},
		       T                                  max_iters = numeric_limits<T>::max());
