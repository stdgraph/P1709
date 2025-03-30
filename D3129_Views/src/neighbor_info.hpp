template <class VorVId, bool Sourced, class VV>
struct neighbor_info {
  using source_type = VorVId; // e.g. vertex\_t<G> or vertex\_id\_t<G> when Sourced==true, or void
  using target_type = VorVId; // e.g. vertex\_t<G> or vertex\_id\_t<G>
  using value_type  = VV;     // e.g. vertex\_value\_t<G> or void

  source_type source;
  target_type target;
  value_type  value;
};
