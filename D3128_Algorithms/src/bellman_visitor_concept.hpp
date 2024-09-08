template <class G, class Visitor>
concept bellman_visitor =
      requires(Visitor&                                             v,
               remove_reference_t<Visitor>::vertex_desc_type&       vdesc,
               remove_reference_t<Visitor>::sourced_edge_desc_type& edesc) {
        { v.on_examine_edge(edesc) };
        { v.on_edge_relaxed(edesc) };
        { v.on_edge_not_relaxed(edesc) };
        { v.on_edge_minimized(edesc) };
        { v.on_edge_not_minimized(edesc) };
      };
