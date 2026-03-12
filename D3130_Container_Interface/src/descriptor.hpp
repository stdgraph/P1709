// For exposition only

struct out_edge_tag {};
struct in_edge_tag {};

template <class VertexIter>
class vertex_descriptor {
public:
  using iterator_type = VertexIter;
  using value_type    = iter_value_t<VertexIter>;

  // index for random-access, iterator for bidirectional (for exposition only)
  using storage_type =
      conditional_t<random_access_iterator<VertexIter>, size_t, VertexIter>;

  constexpr vertex_descriptor() = default;
  constexpr explicit vertex_descriptor(storage_type val) noexcept;

  // Properties
  constexpr storage_type      value() const noexcept;
  constexpr decltype(auto)    vertex_id() const noexcept;

  template <class Container>
  constexpr decltype(auto) underlying_value(Container& c) const noexcept;

  template <class Container>
  constexpr decltype(auto) inner_value(Container& c) const noexcept;

  // Iterator-like interface
  constexpr vertex_descriptor& operator++() noexcept;
  constexpr vertex_descriptor  operator++(int) noexcept;

  // Comparison
  constexpr auto operator<=>(const vertex_descriptor&) const noexcept = default;
  constexpr bool operator==(const vertex_descriptor&) const noexcept  = default;

private:
  storage_type storage_ = storage_type(); // for exposition only
};


template <class EdgeIter,
          class VertexIter,
          class EdgeDirection = out_edge_tag>
class edge_descriptor {
public:
  using edge_iterator_type   = EdgeIter;
  using vertex_iterator_type = VertexIter;
  using vertex_desc          = vertex_descriptor<VertexIter>;
  using edge_direction       = EdgeDirection;

  static constexpr bool is_in_edge  = is_same_v<EdgeDirection, in_edge_tag>;
  static constexpr bool is_out_edge = is_same_v<EdgeDirection, out_edge_tag>;

  // index for random-access, iterator for forward (for exposition only)
  using edge_storage_type =
      conditional_t<random_access_iterator<EdgeIter>, size_t, EdgeIter>;

  constexpr edge_descriptor() = default;
  constexpr edge_descriptor(edge_storage_type edge_val,
                            vertex_desc source) noexcept;

  // Properties
  constexpr edge_storage_type value() const noexcept;
  constexpr vertex_desc       source() const noexcept;
  constexpr decltype(auto)    source_id() const noexcept;

  template <class VertexData>
  constexpr auto target_id(const VertexData& vd) const noexcept;

  template <class VertexData>
  constexpr decltype(auto) underlying_value(VertexData& vd) const noexcept;

  // Comparison
  constexpr auto operator<=>(const edge_descriptor&) const noexcept = default;
  constexpr bool operator==(const edge_descriptor&) const noexcept  = default;

private:
  edge_storage_type edge_storage_ = edge_storage_type(); // for exposition only
  vertex_desc       source_       = vertex_desc();       // for exposition only
};
