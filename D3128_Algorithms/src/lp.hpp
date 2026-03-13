/*
 * Label Propagation Algorithm
 */
template <adjacency_list G,
          class          Label,
          class Gen = default_random_engine,
          class T   = size_t>
requires vertex_property_map_for<Label, G> &&
         equality_comparable<vertex_property_map_value_t<Label>> &&
         uniform_random_bit_generator<remove_cvref_t<Gen>>
void label_propagation(G&&    g,
                       Label& label,
                       Gen&&  rng       = default_random_engine{},
                       T      max_iters = numeric_limits<T>::max());

template <adjacency_list G,
          class          Label,
          class Gen = default_random_engine,
          class T   = size_t>
requires vertex_property_map_for<Label, G> &&
         equality_comparable<vertex_property_map_value_t<Label>> &&
         uniform_random_bit_generator<remove_cvref_t<Gen>>
void label_propagation(G&&                                   g,
                       Label&                                label,
                       vertex_property_map_value_t<Label>    empty_label,
                       Gen&&                                 rng       = default_random_engine{},
                       T                                     max_iters = numeric_limits<T>::max());
