// Primary template
template <class VId, class V, class VV>
struct vertex_data {
  using id_type     = VId; // e.g. vertex\_id\_t<G> or void
  using vertex_type = V;   // e.g. vertex\_t<G> or void
  using value_type  = VV;  // e.g. invoke\_result of vvf, or void

  id_type     id;
  vertex_type vertex;
  value_type  value;
};

// Specialization: no value function
template <class VId, class V>
struct vertex_data<VId, V, void> {
  using id_type     = VId;
  using vertex_type = V;

  id_type     id;
  vertex_type vertex;
};

// Specialization: no vertex descriptor
template <class VId, class VV>
struct vertex_data<VId, void, VV> {
  using id_type    = VId;
  using value_type = VV;

  id_type    id;
  value_type value;
};

// Specialization: id only
template <class VId>
struct vertex_data<VId, void, void> {
  using id_type = VId;

  id_type id;
};
