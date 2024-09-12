template <class G, class Visitor>
concept has_on_examine_edge = //
      requires(Visitor& v, edge_descriptor<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_examine_edge(edesc) };
      };
template <class G, class Visitor>
concept has_on_edge_relaxed = //
      requires(Visitor& v, edge_descriptor<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_edge_relaxed(edesc) };
      };
template <class G, class Visitor>
concept has_on_edge_not_relaxed = //
      requires(Visitor& v, edge_descriptor<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_edge_not_relaxed(edesc) };
      };
template <class G, class Visitor>
concept has_on_edge_minimized = //
      requires(Visitor& v, edge_descriptor<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_edge_minimized(edesc) };
      };
template <class G, class Visitor>
concept has_on_edge_not_minimized =
      requires(Visitor& v, edge_descriptor<vertex_id_t<G>, true, edge_reference_t<G>, void> edesc) {
        { v.on_edge_not_minimized(edesc) };
      };
