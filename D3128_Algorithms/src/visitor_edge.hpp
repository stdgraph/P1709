template <class G, class Visitor>
concept has_on_examine_edge = // For exposition only
      requires(Visitor& v, const G& g, const edge_t<G>& e) {
        { v.on_examine_edge(g, e) };
      };
template <class G, class Visitor>
concept has_on_edge_relaxed = // For exposition only
      requires(Visitor& v, const G& g, const edge_t<G>& e) {
        { v.on_edge_relaxed(g, e) };
      };
template <class G, class Visitor>
concept has_on_edge_not_relaxed = // For exposition only
      requires(Visitor& v, const G& g, const edge_t<G>& e) {
        { v.on_edge_not_relaxed(g, e) };
      };
template <class G, class Visitor>
concept has_on_edge_minimized = // For exposition only
      requires(Visitor& v, const G& g, const edge_t<G>& e) {
        { v.on_edge_minimized(g, e) };
      };
template <class G, class Visitor>
concept has_on_edge_not_minimized = // For exposition only
      requires(Visitor& v, const G& g, const edge_t<G>& e) {
        { v.on_edge_not_minimized(g, e) };
      };
template <class G, class Visitor>
concept has_on_tree_edge = // For exposition only
      requires(Visitor& v, const G& g, const edge_t<G>& e) {
        { v.on_tree_edge(g, e) };
      };
template <class G, class Visitor>
concept has_on_back_edge = // For exposition only
      requires(Visitor& v, const G& g, const edge_t<G>& e) {
        { v.on_back_edge(g, e) };
      };
template <class G, class Visitor>
concept has_on_forward_or_cross_edge = // For exposition only
      requires(Visitor& v, const G& g, const edge_t<G>& e) {
        { v.on_forward_or_cross_edge(g, e) };
      };
template <class G, class Visitor>
concept has_on_finish_edge = // For exposition only
      requires(Visitor& v, const G& g, const edge_t<G>& e) {
        { v.on_finish_edge(g, e) };
      };
