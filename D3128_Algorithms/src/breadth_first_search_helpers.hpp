template <class DistanceValue>
constexpr auto breadth_first_search_infinite_distance() {
  return numeric_limits<DistanceValue>::max(); // exposition only
}

template <class DistanceValue>
constexpr auto breadth_first_search_zero() { return DistanceValue(); } // exposition only

template <class Distances>
constexpr void init_breadth_first_search(Distances& distances) {
  // exposition only
  fill(distances, breadth_first_search_infinite_distance<range_value_t<Distances>>());
}

template <class Predecessors>
constexpr void init_breadth_first_search(Predecessors& predecessors) {
  ranges::iota(predecessors, 0); // exposition only
}
