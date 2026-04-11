// General concept: a callable returning a mutable lvalue reference to any per-vertex property value
// For exposition only
template <class VF, class G>
concept vertex_property_fn_for =
      invocable<VF&, const remove_reference_t<G>&, const vertex_id_t<G>&> &&
      is_lvalue_reference_v<invoke_result_t<VF&, const remove_reference_t<G>&, const vertex_id_t<G>&>>;

// Type alias: extracts the value type from a vertex property function's return type
// For exposition only
template <class VF, class G>
using vertex_fn_value_t = remove_cvref_t<
    invoke_result_t<VF&, const remove_reference_t<G>&, const vertex_id_t<G>&>>;

// Concept: a callable returning a mutable reference to a per-vertex distance value
// For exposition only
template <class DF, class G>
concept distance_fn_for = vertex_property_fn_for<DF, G>;

// Type alias: extracts the distance value type from a distance function's return type
// For exposition only
template <class DF, class G>
using distance_fn_value_t = vertex_fn_value_t<DF, G>;

// Concept: a callable returning a mutable reference to a per-vertex predecessor value
// For exposition only
template <class PF, class G>
concept predecessor_fn_for = vertex_property_fn_for<PF, G>;

// Type alias: extracts the predecessor value type from a predecessor function's return type
// For exposition only
template <class PF, class G>
using predecessor_fn_value_t = vertex_fn_value_t<PF, G>;

// Null predecessor function — used when predecessor tracking is not needed
// For exposition only
struct _null_predecessor_fn {
  template <class G, class VId>
  size_t& operator()(const G&, const VId&);
};
inline _null_predecessor_fn _null_predecessor; // global instance

// Adapts a subscriptable container into a property function: fn(g, uid) returns container[uid] by ref
// For exposition only
template <class Container>
struct container_value_fn {
  Container& c;
  template <class G, class VId>
  constexpr auto& operator()(const G&, const VId& uid) const { return c[uid]; }
};
template <class Container>
container_value_fn(Container&) -> container_value_fn<Container>;

template <class DistanceValue>
constexpr auto infinite_distance() {
  return numeric_limits<DistanceValue>::max(); // exposition only
}

template <class DistanceValue>
constexpr auto zero_distance() {
  return DistanceValue(); // exposition only
}

template <class G, class Distances>
requires adjacency_list<G> && vertex_property_map_for<Distances, G>
constexpr void init_shortest_paths(const G& g, Distances& distances) {
  // exposition only: sets all entries to infinite distance
}

template <class G, class Distances, class Predecessors>
requires adjacency_list<G> && vertex_property_map_for<Distances, G> &&
         vertex_property_map_for<Predecessors, G>
constexpr void init_shortest_paths(const G& g, Distances& distances, Predecessors& predecessors) {
  // exposition only: sets distances to infinite distance, predecessors[v] = v
}
