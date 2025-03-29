template <class VorVId, class V, class VV>
struct vertex_info {
  using vertex_type = VorVId; // e.g. vertex\_reference\_t<G> or void
  using value_type  = VV;     // e.g. vertex\_value\_t<G> or void

  vertex_type vertex;
  value_type  value;
};
