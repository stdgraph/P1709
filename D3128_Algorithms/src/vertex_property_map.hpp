// vertex\_property\_map: per-vertex associative container
// vector for index graphs, unordered\_map for mapped graphs.
template <class G, class T>
using vertex_property_map = conditional_t<
    index_vertex_range<G>,
    vector<T>,
    unordered_map<vertex_id_t<G>, T>>;

// Eager initialization: every vertex pre-populated with init\_value.  $\mathcal{O}(V)$.
template <class G, class T>
constexpr auto make_vertex_property_map(const G& g, const T& init_value);

// Lazy initialization: sized for index graphs, empty and reserved for mapped graphs.
template <class G, class T>
constexpr auto make_vertex_property_map(const G& g);

// Test whether a vertex ID has an entry in the map.
template <class Map, class Key>
constexpr bool vertex_property_map_contains(const Map& m, const Key& uid);

// Read with default fallback; no insertion for unordered\_map.
template <class Map, class Key, class T>
constexpr auto vertex_property_map_get(const Map& m, const Key& uid,
                                       const T& default_val);

// Extract the per-vertex value type from a vertex\_property\_map container.
// For vector: value\_type.  For unordered\_map: mapped\_type.
template <class Container>
using vertex_property_map_value_t =
    typename detail::vertex_property_map_value_impl<Container>::type;

// Concept: container usable as a per-vertex property map for graph G.
// Requires subscript access with the graph's vertex ID type, returning a value
// convertible to the map's value type.
template <class M, class G>
concept vertex_property_map_for = requires(M& m, const vertex_id_t<G>& uid) {
  { m[uid] } -> convertible_to<vertex_property_map_value_t<M>>;
};
