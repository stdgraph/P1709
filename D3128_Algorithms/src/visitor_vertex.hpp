template <class G, class Visitor>
concept has_on_initialize_vertex = // For exposition only
      requires(Visitor& v, const G& g, const vertex_t<G>& vdesc) {
        { v.on_initialize_vertex(g, vdesc) };
      };
template <class G, class Visitor>
concept has_on_initialize_vertex_id = // For exposition only
      requires(Visitor& v, const G& g, const vertex_id_t<G>& uid) {
        { v.on_initialize_vertex(g, uid) };
      };
template <class G, class Visitor>
concept has_on_discover_vertex = // For exposition only
      requires(Visitor& v, const G& g, const vertex_t<G>& vdesc) {
        { v.on_discover_vertex(g, vdesc) };
      };
template <class G, class Visitor>
concept has_on_discover_vertex_id = // For exposition only
      requires(Visitor& v, const G& g, const vertex_id_t<G>& uid) {
        { v.on_discover_vertex(g, uid) };
      };
template <class G, class Visitor>
concept has_on_start_vertex = // For exposition only
      requires(Visitor& v, const G& g, const vertex_t<G>& vdesc) {
        { v.on_start_vertex(g, vdesc) };
      };
template <class G, class Visitor>
concept has_on_start_vertex_id = // For exposition only
      requires(Visitor& v, const G& g, const vertex_id_t<G>& uid) {
        { v.on_start_vertex(g, uid) };
      };
template <class G, class Visitor>
concept has_on_examine_vertex = // For exposition only
      requires(Visitor& v, const G& g, const vertex_t<G>& vdesc) {
        { v.on_examine_vertex(g, vdesc) };
      };
template <class G, class Visitor>
concept has_on_examine_vertex_id = // For exposition only
      requires(Visitor& v, const G& g, const vertex_id_t<G>& uid) {
        { v.on_examine_vertex(g, uid) };
      };
template <class G, class Visitor>
concept has_on_finish_vertex = // For exposition only
      requires(Visitor& v, const G& g, const vertex_t<G>& vdesc) {
        { v.on_finish_vertex(g, vdesc) };
      };
template <class G, class Visitor>
concept has_on_finish_vertex_id = // For exposition only
      requires(Visitor& v, const G& g, const vertex_id_t<G>& uid) {
        { v.on_finish_vertex(g, uid) };
      };
