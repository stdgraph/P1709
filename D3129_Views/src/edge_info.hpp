template <class VorVId, bool Sourced, class E, class EV>
struct edge_info {
  using source_type = VorVId;  // e.g. vertex\_t<G> or vertex\_id\_t<G> when Sourced==true, or void
  using target_type = VorVId;  // e.g. vertex\_t<G> or vertex\_id\_t<G> 
  using edge_type   = E;       // e.g. edge\_reference\_t<G> or void
  using value_type  = EV;      // e.g. edge\_value\_t<G> or void

  source_type source;
  target_type target;
  edge_type   edge;
  value_type  value;
};
