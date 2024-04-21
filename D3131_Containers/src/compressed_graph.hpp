template <class EV, 
          class VV, 
          class GV, 
          integral VId=uint32_t, 
          integral EIndex=uint32_t, 
          class    Alloc=allocator<VId>>
class compressed_graph {
public: // Construction/Destruction/Assignment
  constexpr compressed_graph()                        = default;
  constexpr compressed_graph(const compressed_graph&) = default;
  constexpr compressed_graph(compressed_graph&&)      = default;
  constexpr ~compressed_graph()                       = default;

  constexpr compressed_graph& operator=(const compressed_graph&) = default;
  constexpr compressed_graph& operator=(compressed_graph&&)      = default;

  // compressed\_graph(      alloc)
  // compressed\_graph(gv\&,  alloc)
  // compressed\_graph(gv\&\&, alloc)

  constexpr compressed_graph(const Alloc& alloc);
  constexpr compressed_graph(const graph_value_type& value, const Alloc& alloc = Alloc());
  constexpr compressed_graph(graph_value_type&& value, const Alloc& alloc = Alloc());

  // compressed\_graph(erng, eprojection, alloc)
  // compressed\_graph(gv\&,  erng, eprojection, alloc)
  // compressed\_graph(gv\&\&, erng, eprojection, alloc)

  template <ranges::forward_range ERng, ranges::forward_range PartRng, class EProj = identity>
  requires copyable_edge<invoke_result<EProj, ranges::range_value_t<ERng>>, VId, EV> &&
           convertible_to<ranges::range_value_t<PartRng>, VId>
  constexpr compressed_graph(const ERng&    erng,
                             EProj          eprojection,
                             const PartRng& part_start_ids = vector<VId>(),
                             const Alloc&   alloc          = Alloc());

  template <ranges::forward_range ERng, ranges::forward_range PartRng, class EProj = identity>
  requires copyable_edge<invoke_result<EProj, ranges::range_value_t<ERng>>, VId, EV> &&
                 convertible_to<ranges::range_value_t<PartRng>, VId>
  constexpr compressed_graph(const graph_value_type& value,
                             const ERng&             erng,
                             EProj                   eprojection,
                             const PartRng&          part_start_ids = vector<VId>(),
                             const Alloc&            alloc          = Alloc());

  template <ranges::forward_range ERng, ranges::forward_range PartRng, class EProj = identity>
  requires copyable_edge<invoke_result<EProj, ranges::range_value_t<ERng>>, VId, EV> &&
                 convertible_to<ranges::range_value_t<PartRng>, VId>
  constexpr compressed_graph(graph_value_type&& value,
                             const ERng&        erng,
                             EProj              eprojection,
                             const PartRng&     part_start_ids = vector<VId>(),
                             const Alloc&       alloc          = Alloc());

  // compressed\_graph(erng, vrng, eprojection, vprojection, alloc)
  // compressed\_graph(gv\&,  erng, vrng, eprojection, vprojection, alloc)
  // compressed\_graph(gv\&\&, erng, vrng, eprojection, vprojection, alloc)

  template <ranges::forward_range ERng,
            ranges::forward_range VRng,
            ranges::forward_range PartRng,
            class EProj = identity,
            class VProj = identity>
  requires copyable_edge<invoke_result<EProj, ranges::range_value_t<ERng>>, VId, EV> &&
           copyable_vertex<invoke_result<VProj, ranges::range_value_t<VRng>>, VId, VV> &&
           convertible_to<ranges::range_value_t<PartRng>, VId>
  constexpr compressed_graph(const ERng&    erng,
                             const VRng&    vrng,
                             EProj          eprojection    = {},
                             VProj          vprojection    = {},
                             const PartRng& part_start_ids = vector<VId>(),
                             const Alloc&   alloc          = Alloc());

  template <ranges::forward_range ERng,
            ranges::forward_range VRng,
            ranges::forward_range PartRng,
            class EProj = identity,
            class VProj = identity>
  requires copyable_edge<invoke_result<EProj, ranges::range_value_t<ERng>>, VId, EV> &&
                 copyable_vertex<invoke_result<VProj, ranges::range_value_t<VRng>>, VId, VV> &&
                 convertible_to<ranges::range_value_t<PartRng>, VId>
  constexpr compressed_graph(const graph_value_type& value,
                             const ERng&             erng,
                             const VRng&             vrng,
                             EProj                   eprojection    = {},
                             VProj                   vprojection    = {},
                             const PartRng&          part_start_ids = vector<VId>(),
                             const Alloc&            alloc          = Alloc());

  template <ranges::forward_range ERng,
            ranges::forward_range VRng,
            ranges::forward_range PartRng,
            class EProj = identity,
            class VProj = identity>
  requires copyable_edge<invoke_result<EProj, ranges::range_value_t<ERng>>, VId, EV> &&
                 copyable_vertex<invoke_result<VProj, ranges::range_value_t<VRng>>, VId, VV> &&
                 convertible_to<ranges::range_value_t<PartRng>, VId>
  constexpr compressed_graph(graph_value_type&& value,
                             const ERng&        erng,
                             const VRng&        vrng,
                             EProj              eprojection    = {},
                             VProj              vprojection    = {},
                             const PartRng&     part_start_ids = vector<VId>(),
                             const Alloc&       alloc          = Alloc());

  constexpr compressed_graph(const initializer_list<copyable_edge_t<VId, EV>>& ilist, 
                             const Alloc& alloc = Alloc());
};
