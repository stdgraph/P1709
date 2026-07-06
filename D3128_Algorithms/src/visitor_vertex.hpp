template <class G, class Visitor>
concept has_on_initialize_vertex = // For exposition only
      requires(Visitor& v, const G& g, const vertex_t<G>& u) {
        { v.on_initialize_vertex(g, u) };
      };
template <class G, class Visitor>
concept has_on_discover_vertex = // For exposition only
      requires(Visitor& v, const G& g, const vertex_t<G>& u) {
        { v.on_discover_vertex(g, u) };
      };
template <class G, class Visitor>
concept has_on_start_vertex = // For exposition only
      requires(Visitor& v, const G& g, const vertex_t<G>& u) {
        { v.on_start_vertex(g, u) };
      };
template <class G, class Visitor>
concept has_on_examine_vertex = // For exposition only
      requires(Visitor& v, const G& g, const vertex_t<G>& u) {
        { v.on_examine_vertex(g, u) };
      };
template <class G, class Visitor>
concept has_on_finish_vertex = // For exposition only
      requires(Visitor& v, const G& g, const vertex_t<G>& u) {
        { v.on_finish_vertex(g, u) };
      };
