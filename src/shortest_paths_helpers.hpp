template <class DistanceValue>
auto shortest_path_invalid_distance() {
  return std::numeric_limits<DistanceValue>::max(); // exposition only
}

template <class DistanceValue>
auto shortest_path_zero() {
  return {}; // exposition only
}

template <class Predecessors, class Distances>
void init_shortest_paths(Predecessors&& predecessors, Distances&& distances);

template <class Predecessors, class Distances>
void init_shortest_paths(Predecessors&                    predecessors,
                         Distances&                       distances,
                         ranges::range_value_t<Distances> init);

template <class Distances>
void init_shortest_distances(Distances& distances);

template <class Distances>
void init_shortest_distances(Distances& distances, ranges::range_value_t<Distances> init);
