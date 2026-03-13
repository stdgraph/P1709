// Primary template
template <class VId, bool Sourced, class V, class VV>
struct neighbor_data {
  using source_id_type = VId; // vertex\_id\_t<G> when Sourced==true, or void
  using target_id_type = VId; // vertex\_id\_t<G>
  using vertex_type    = V;   // vertex\_t<G> or void
  using value_type     = VV;  // invoke\_result of vvf, or void

  source_id_type source_id;
  target_id_type target_id;
  vertex_type    target;
  value_type     value;
};

// Specialization: neighbors (unsourced, no value)
template <class VId, class V>
struct neighbor_data<VId, false, V, void> {
  using target_id_type = VId;
  using vertex_type    = V;

  target_id_type target_id;
  vertex_type    target;
};

// Specialization: basic neighbors (unsourced, no descriptor, no value)
template <class VId>
struct neighbor_data<VId, false, void, void> {
  using target_id_type = VId;

  target_id_type target_id;
};

// Specialization: neighbors with value (unsourced)
template <class VId, class V, class VV>
struct neighbor_data<VId, false, V, VV> {
  using target_id_type = VId;
  using vertex_type    = V;
  using value_type     = VV;

  target_id_type target_id;
  vertex_type    target;
  value_type     value;
};
