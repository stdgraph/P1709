// For exposition only

template <class VertexIter>
class vertex_descriptor_view : public view_interface<vertex_descriptor_view<VertexIter>> {
public:
  using vertex_desc        = vertex_descriptor<VertexIter>;
  using storage_type       = typename vertex_desc::storage_type;
  using underlying_iterator = VertexIter;

  class iterator; // forward iterator yielding vertex\_desc by value
  using const_iterator = iterator;

  constexpr vertex_descriptor_view() = default;

  template <class Container>
  constexpr explicit vertex_descriptor_view(Container& c);

  constexpr iterator begin() const noexcept;
  constexpr iterator end() const noexcept;
  constexpr size_t   size() const noexcept;
};


template <class EdgeIter,
          class VertexIter,
          class EdgeDirection = out_edge_tag>
class edge_descriptor_view
    : public view_interface<edge_descriptor_view<EdgeIter, VertexIter, EdgeDirection>> {
public:
  using edge_desc  = edge_descriptor<EdgeIter, VertexIter, EdgeDirection>;
  using vertex_desc = vertex_descriptor<VertexIter>;

  class iterator; // forward iterator yielding edge\_desc by value
  using const_iterator = iterator;

  constexpr edge_descriptor_view() = default;

  template <class Container>
  constexpr edge_descriptor_view(Container& c, vertex_desc source);

  constexpr iterator begin() const noexcept;
  constexpr iterator end() const noexcept;
  constexpr size_t   size() const noexcept;
};
