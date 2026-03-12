// For exposition only

template <class G, class E>
concept edge = requires(G& g, const E& e) {
  source_id(g, e);
  source(g, e);
  target_id(g, e);
  target(g, e);
};
