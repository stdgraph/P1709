template <class VId, bool Sourced, class V, class VV>
struct neighbor_descriptor {
  using source_id_type = VId; // e.g. vertex\_id\_t<G> when Sourced==true, or void
  using target_id_type = VId; // e.g. vertex\_id\_t<G>
  using vertex_type    = V;   // e.g. vertex\_reference\_t<G> or void
  using value_type     = VV;  // e.g. vertex\_value\_t<G> or void

  source_id_type source_id;
  target_id_type target_id;
  vertex_type    target;
  value_type     value;
};
