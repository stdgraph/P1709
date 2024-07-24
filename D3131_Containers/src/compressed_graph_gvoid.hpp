template <class EV, 
          class VV, 
          integral VId=uint32_t, 
          integral EIndex=uint32_t, 
          class    Alloc=allocator<VId>>
template <class EV, class VV, integral VId, integral EIndex, class Alloc>
class compressed_graph<EV, VV, void, VId, EIndex, Alloc>
public: // Construction/Destruction
  constexpr compressed_graph()                        = default;
  constexpr compressed_graph(const compressed_graph&) = default;
  constexpr compressed_graph(compressed_graph&&)      = default;
  constexpr ~compressed_graph()                       = default;

  constexpr compressed_graph& operator=(const compressed_graph&) = default;
  constexpr compressed_graph& operator=(compressed_graph&&)      = default;

  // edge-only construction
  template <ranges::forward_range ERng, 
            class                 EProj = identity, 
            ranges::forward_range PartRng = vector<VId>>
  requires copyable_edge<invoke_result_t<EProj, ranges::range_value_t<ERng>>, VId, EV>
  constexpr compressed_graph(const ERng&    erng,
                             EProj          eprojection         = identity(),
                             const PartRng& partition_start_ids = vector<VId>(),
                             const Alloc&   alloc               = Alloc())

  // edge and vertex value construction
  template <ranges::forward_range ERng,
            ranges::forward_range VRng,
            ranges::forward_range PartRng,
            class EProj = identity,
            class VProj = identity>
  constexpr compressed_graph(const ERng&    erng,
                             const VRng&    vrng,
                             EProj          eprojection    = {},
                             VProj          vprojection    = {},
                             const PartRng& partition_start_ids = vector<VId>(),
                             const Alloc&   alloc          = Alloc());

  // initializer list using edge\_descriptor<VId,true,void,EV>
  constexpr compressed_graph(const initializer_list<copyable_edge_t<VId, EV>>& ilist, 
                             const Alloc& alloc = Alloc());
};
