// For exposition only

// Shared edge floor (namespace std::graph): ids only.
template <class G, class E>
concept basic_edge = requires(G& g, const E& uv) {
  source_id(g, uv); // returns vertex\_id\_t<G>
  target_id(g, uv); // returns vertex\_id\_t<G>
};

// Adjacency-list refinement (namespace std::graph::adj\_list):
// adds the source/target vertex descriptors.
template <class G, class E>
concept edge = basic_edge<G, E> && is_edge_descriptor_v<E> && requires(G& g, const E& uv) {
  source(g, uv); // returns vertex descriptor vertex\_t<G>
  target(g, uv); // returns vertex descriptor vertex\_t<G>
};
