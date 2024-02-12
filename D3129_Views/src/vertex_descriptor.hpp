template <class VId, class V, class VV>
struct vertex_descriptor {
  using id_type     = VId; // e.g. vertex\_id\_t<G>
  using vertex_type = V;   // e.g. vertex\_reference\_t<G> or void
  using value_type  = VV;  // e.g. vertex\_value\_t<G> or void

  id_type     id;
  vertex_type vertex;
  value_type  value;
};
