// Primary template
template <class VId, bool Sourced, class E, class EV>
struct edge_data {
  using source_id_type = VId; // vertex\_id\_t<G> when Sourced==true, or void
  using target_id_type = VId; // vertex\_id\_t<G>
  using edge_type      = E;   // edge\_t<G> or void
  using value_type     = EV;  // invoke\_result of evf, or void

  source_id_type source_id;
  target_id_type target_id;
  edge_type      edge;
  value_type     value;
};

// Specialization: incidence (unsourced, no value)
template <class VId, class E>
struct edge_data<VId, false, E, void> {
  using target_id_type = VId;
  using edge_type      = E;

  target_id_type target_id;
  edge_type      edge;
};

// Specialization: edgelist (sourced, no value)
template <class VId, class E>
struct edge_data<VId, true, E, void> {
  using source_id_type = VId;
  using target_id_type = VId;
  using edge_type      = E;

  source_id_type source_id;
  target_id_type target_id;
  edge_type      edge;
};

// Specialization: basic incidence (unsourced, no descriptor, no value)
template <class VId>
struct edge_data<VId, false, void, void> {
  using target_id_type = VId;

  target_id_type target_id;
};

// Specialization: search edge views (no id, unsourced)
template <class E>
struct edge_data<void, false, E, void> {
  using edge_type = E;

  edge_type edge;
};
