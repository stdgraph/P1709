template <class G, class Visitor>
concept has_on_initialize_vertex = //
      requires(Visitor& v, vertex_descriptor<vertex_id_t<G>, vertex_reference_t<G>, void> vdesc) {
        { v.on_initialize_vertex(vdesc) };
      };
template <class G, class Visitor>
concept has_on_discover_vertex = //
      requires(Visitor& v, vertex_descriptor<vertex_id_t<G>, vertex_reference_t<G>, void> vdesc) {
        { v.on_discover_vertex(vdesc) };
      };
template <class G, class Visitor>
concept has_on_examine_vertex = //
      requires(Visitor& v, vertex_descriptor<vertex_id_t<G>, vertex_reference_t<G>, void> vdesc) {
        { v.on_examine_vertex(vdesc) };
      };
template <class G, class Visitor>
concept has_on_finish_vertex = //
      requires(Visitor& v, vertex_descriptor<vertex_id_t<G>, vertex_reference_t<G>, void> vdesc) {
        { v.on_finish_vertex(vdesc) };
      };
