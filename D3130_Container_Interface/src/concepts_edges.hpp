// For exposition only

// Shared edge floor (namespace std::graph): ids only.
template <class G, class E>
concept basic_edge = requires(G& g, const E& e) {
  source_id(g, e);
  target_id(g, e);
};

// Adjacency-list refinement (namespace std::graph::adj\_list):
// adds the source/target vertex descriptors.
template <class G, class E>
concept edge = basic_edge<G, E> && requires(G& g, const E& e) {
  source(g, e);
  target(g, e);
};
