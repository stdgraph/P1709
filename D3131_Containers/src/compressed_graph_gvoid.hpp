template <class EV, class VV, integral VId, integral EIndex, class Alloc>
class compressed_graph<EV, VV, void, VId, EIndex, Alloc>
      : public compressed_graph_base<EV, VV, void, VId, EIndex, Alloc> {
public: // Construction/Destruction
  constexpr compressed_graph()                        = default;
  constexpr compressed_graph(const compressed_graph&) = default;
  constexpr compressed_graph(compressed_graph&&)      = default;
  constexpr ~compressed_graph()                       = default;

  constexpr compressed_graph& operator=(const compressed_graph&) = default;
  constexpr compressed_graph& operator=(compressed_graph&&)      = default;

  // edge-only construction
  template <forward_range ERng, 
            class                 EProj = identity, 
            forward_range PartRng = vector<VId>>
  requires copyable_edge<invoke_result_t<EProj, range_value_t<ERng>>, VId, EV>
  constexpr compressed_graph(const ERng&    erng,
                             EProj          eprojection         = identity(),
                             const PartRng& partition_start_ids = vector<VId>(),
                             const Alloc&   alloc               = Alloc());

  // edge and vertex value construction
  template <forward_range ERng,
            forward_range VRng,
            class EProj           = identity,
            class VProj           = identity,
            forward_range PartRng = vector<VId>>
  requires copyable_edge<invoke_result_t<EProj, range_value_t<ERng>>, VId, EV> &&
           copyable_vertex<invoke_result_t<VProj, range_value_t<VRng>>, VId, VV> &&
           convertible_to<range_value_t<PartRng>, VId>
  constexpr compressed_graph(const ERng&    erng,
                             const VRng&    vrng,
                             EProj          eprojection    = {},
                             VProj          vprojection    = {},
                             const PartRng& partition_start_ids = vector<VId>(),
                             const Alloc&   alloc          = Alloc());

  // initializer list using edge\_data<VId,true,void,EV>
  constexpr compressed_graph(const initializer_list<copyable_edge_t<VId, EV>>& ilist, 
                             const Alloc& alloc = Alloc());
};
