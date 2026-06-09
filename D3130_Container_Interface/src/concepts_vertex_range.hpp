// For exposition only

template <class G, class V>
concept vertex = is_vertex_descriptor_v<remove_cvref_t<V>> &&
                 requires(G& g, const V& u, const vertex_id_t<G>& uid) {
                   vertex_id(g, u);
                   find_vertex(g, uid);
                 };

template <class R, class G>
concept vertex_range = forward_range<R> &&
                       sized_range<R> &&
                       vertex<G, remove_cvref_t<range_value_t<R>>>;

template <class G>
concept index_vertex_range =
    integral<vertex_id_t<G>> &&
    integral<typename vertex_range_t<G>::storage_type> &&
    requires(G& g) {
      { vertices(g) } -> vertex_range<G>;
    };

// vertex id usable as an unordered\_map key (for mapped property maps)
template <class G>
concept hashable_vertex_id = requires(const vertex_id_t<G>& uid) {
  { hash<vertex_id_t<G>>{}(uid) } -> convertible_to<size_t>;
};

template <class G>
concept mapped_vertex_range =
    !index_vertex_range<G> &&
    hashable_vertex_id<G> &&
    requires(G& g, const vertex_id_t<G>& uid) {
      { vertices(g) } -> forward_range;
      find_vertex(g, uid);
    };
