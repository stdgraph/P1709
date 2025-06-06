template <class G, class Visitor>
concept has_on_initialize_vertex = // For exposition only
      requires(Visitor& v, vertex_info<vertex_reference_t<G>, void> vdesc) {
        { v.on_initialize_vertex(vdesc) };
      };
template <class G, class Visitor>
concept has_on_discover_vertex = // For exposition only
      requires(Visitor& v, vertex_info<vertex_reference_t<G>, void> vdesc) {
        { v.on_discover_vertex(vdesc) };
      };
template <class G, class Visitor>
concept has_on_start_vertex = // For exposition only
      requires(Visitor& v, vertex_info<vertex_reference_t<G>, void> vdesc) {
        { v.on_start_vertex(vdesc) };
      };
template <class G, class Visitor>
concept has_on_examine_vertex = // For exposition only
      requires(Visitor& v, vertex_info<vertex_reference_t<G>, void> vdesc) {
        { v.on_examine_vertex(vdesc) };
      };
template <class G, class Visitor>
concept has_on_finish_vertex = // For exposition only
      requires(Visitor& v, vertex_info<vertex_reference_t<G>, void> vdesc) {
        { v.on_finish_vertex(vdesc) };
      };
