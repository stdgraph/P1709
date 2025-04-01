// For exposition only

template <class G>
concept targeted_edge_range = 
          basic_targeted_edge_range<G> &&
          requires(G&& g, vertex_reference_t<G> u, vertex_id_t<G> uid) {
            { edges(g, u) } -> ranges::forward_range;
            { edges(g, uid) } -> ranges::forward_range; // implies call to find\_vertex(g,uid)
          };
