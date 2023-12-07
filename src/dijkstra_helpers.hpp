template <class DistanceValue>
auto dijkstra_invalid_distance() {
  return std::numeric_limits<DistanceValue>::max();
}

template <class DistanceValue>
auto dijkstra_zero() {
  return {};
}

template <property P, property D>
void init_dijkstra_shortest_paths(P&& predecessors, D&& distances);

template <property P, property D>
void init_dijkstra_shortest_paths(P&&                           predecessors,
                                  D&&                           distances,
                                  std::ranges::range_value_t<D> init);

template <property D>
void init_dijkstra_shortest_distances(D&& distances);

template <property D>
void init_dijkstra_shortest_distances(D&& distances, std::ranges::range_value_t<D> init);