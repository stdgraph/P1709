template <class VId, bool Sourced, class E, class EV>
struct edge_descriptor {
  using source_id_type = VId; // e.g. vertex\_id\_t<G> when SourceId==true, or void
  using target_id_type = VId; // e.g. vertex\_id\_t<G>
  using edge_type      = E;   // e.g. edge\_reference\_t<G> or void
  using value_type     = EV;  // e.g. edge\_value\_t<G> or void

  source_id_type source_id;
  target_id_type target_id;
  edge_type      edge;
  value_type     value;
};
