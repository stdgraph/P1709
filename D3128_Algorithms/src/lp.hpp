/*
 * Label Propagation Algorithm
 */
template <adjacency_list G,
          class          LabelFn,
          class Gen = default_random_engine,
          class T   = size_t>
requires vertex_property_fn_for<LabelFn, G> &&
         equality_comparable<vertex_fn_value_t<LabelFn, G>> &&
         uniform_random_bit_generator<remove_cvref_t<Gen>>
void label_propagation(G&&      g,
                       LabelFn&& label,
                       Gen&&    rng       = default_random_engine{},
                       T        max_iters = numeric_limits<T>::max());

template <adjacency_list G,
          class          LabelFn,
          class Gen = default_random_engine,
          class T   = size_t>
requires vertex_property_fn_for<LabelFn, G> &&
         equality_comparable<vertex_fn_value_t<LabelFn, G>> &&
         uniform_random_bit_generator<remove_cvref_t<Gen>>
void label_propagation(G&&                           g,
                       LabelFn&&                     label,
                       vertex_fn_value_t<LabelFn, G> empty_label,
                       Gen&&                         rng       = default_random_engine{},
                       T                             max_iters = numeric_limits<T>::max());
