// For exposition only: base concept for any 2-arg edge value function.
template <class WF, class G, class E>
concept edge_value_function = // For exposition only
      std::invocable<WF, const G&, E> && !std::is_void_v<std::invoke_result_t<WF, const G&, E>>;

template <class G, class WF, class DistanceValue, class Compare, class Combine>
// For exposition only
template <class G, class WF, class DistanceValue, class Compare, class Combine>
concept basic_edge_weight_function = // e.g. weight(uv)
      edge_value_function<WF, std::remove_reference_t<G>, edge_t<G>> && 
      strict_weak_order<Compare, DistanceValue, DistanceValue> &&
      assignable_from<add_lvalue_reference_t<DistanceValue>,
            invoke_result_t<Combine, DistanceValue, invoke_result_t<WF, edge_t<G>>>>;

// For exposition only
template <class G, class WF, class DistanceValue>
concept edge_weight_function = // e.g. weight(uv)
      is_arithmetic_v<edge_value_function<WF, std::remove_reference_t<G>, edge_t<G>>> && 
      is_arithmetic_v<DistanceValue> && 
      is_arithmetic_v<invoke_result_t<WF, edge_t<G>>> &&
      basic_edge_weight_function<G,
                                  WF,
                                  DistanceValue,
                                  less<DistanceValue>,
                                  plus<DistanceValue>>;

template <class DistanceValue>
constexpr auto shortest_path_infinite_distance() {
  return numeric_limits<DistanceValue>::max(); // exposition only
}

template <class DistanceValue>
constexpr auto shortest_path_zero() {
  return DistanceValue();
} // exposition only

template <class Distances>
constexpr void init_shortest_paths(Distances& distances) {
  // exposition only
  ranges::fill(distances, shortest_path_infinite_distance<range_value_t<Distances>>());
}

template <class Distances, class Predecessors>
constexpr void init_shortest_paths(Distances& distances, Predecessors& predecessors) {
  // exposition only
  init_shortest_paths(distances);
  ranges::iota(predecessors, 0);
}
