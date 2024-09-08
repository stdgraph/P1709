template <class G, class Visitor>
concept dijkstra_visitor = requires(Visitor&                                             v,
                                    remove_reference_t<Visitor>::vertex_desc_type&       vdesc,
                                    remove_reference_t<Visitor>::sourced_edge_desc_type& edesc) {
  { v.on_initialize_vertex(vdesc) };
  { v.on_discover_vertex(vdesc) };
  { v.on_examine_vertex(vdesc) };
  { v.on_finish_vertex(vdesc) };

  { v.on_examine_edge(edesc) };
  { v.on_edge_relaxed(edesc) };
  { v.on_edge_not_relaxed(edesc) };
};
