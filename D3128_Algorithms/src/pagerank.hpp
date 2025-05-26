template <class ExecutionPolicy,
          adjacency_list_graph        G,
          random_access_range P,
          class EVF = std::function<range_value_t<P>(edge_reference_t<G>)>>
requires random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>> &&
      is_arithmetic_v<range_value_t<P>> && edge_weight_function<G, EVF>
void pagerank(ExecutionPolicy   policy,
              G&&               g,
              P&                scores,
              const double      damping_factor,
              const double      threshold,
              const std::size_t max_iterations,
              EVF               weight_fn) {}

template <adjacency_list_graph        G,
          random_access_range P,
          class EVF = std::function<range_value_t<P>(edge_reference_t<G>)>>
requires random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>> &&
      is_arithmetic_v<range_value_t<P>> && edge_weight_function<G, EVF>
void pagerank(G&&               g,
              P&                scores,
              const double      damping_factor,
              const double      threshold,
              const std::size_t max_iterations,
              EVF               weight_fn) {}