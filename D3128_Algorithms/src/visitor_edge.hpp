template <class G, class Visitor>
concept has_on_examine_edge = // For exposition only
      requires(Visitor& v, edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_examine_edge(edesc) };
      };
template <class G, class Visitor>
concept has_on_edge_relaxed = // For exposition only
      requires(Visitor& v, edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_edge_relaxed(edesc) };
      };
template <class G, class Visitor>
concept has_on_edge_not_relaxed = // For exposition only
      requires(Visitor& v, edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_edge_not_relaxed(edesc) };
      };
template <class G, class Visitor>
concept has_on_edge_minimized = // For exposition only
      requires(Visitor& v, edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_edge_minimized(edesc) };
      };
template <class G, class Visitor>
concept has_on_edge_not_minimized = // For exposition only
      requires(Visitor& v, edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_edge_not_minimized(edesc) };
      };

      template <class G, class Visitor>
concept has_on_tree_edge = // For exposition only
      requires(Visitor& v, edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_tree_edge(edesc) };
      };
template <class G, class Visitor>
concept has_on_back_edge = // For exposition only
      requires(Visitor& v, edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_back_edge(edesc) };
      };
template <class G, class Visitor>
concept has_on_forward_or_cross_edge = // For exposition only
      requires(Visitor& v, edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_forward_or_cross_edge(edesc) };
      };
template <class G, class Visitor>
concept has_on_finish_edge = // For exposition only
      requires(Visitor& v, edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_finish_edge(edesc) };
      };
