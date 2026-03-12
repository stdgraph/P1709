// For exposition only

template <class T> struct is_vertex_descriptor : false_type {};
template <class T> struct is_edge_descriptor   : false_type {};
template <class T> struct is_descriptor        : false_type {};

// Specializations for vertex\_descriptor
template <class VertexIter>
struct is_vertex_descriptor<vertex_descriptor<VertexIter>> : true_type {};
template <class VertexIter>
struct is_descriptor<vertex_descriptor<VertexIter>>        : true_type {};

// Specializations for edge\_descriptor
template <class EdgeIter, class VertexIter, class EdgeDirection>
struct is_edge_descriptor<edge_descriptor<EdgeIter, VertexIter, EdgeDirection>>
    : true_type {};
template <class EdgeIter, class VertexIter, class EdgeDirection>
struct is_descriptor<edge_descriptor<EdgeIter, VertexIter, EdgeDirection>>
    : true_type {};

// Helper variable templates
template <class T>
inline constexpr bool is_vertex_descriptor_v = is_vertex_descriptor<remove_cv_t<T>>::value;

template <class T>
inline constexpr bool is_edge_descriptor_v = is_edge_descriptor<remove_cv_t<T>>::value;

template <class T>
inline constexpr bool is_descriptor_v = is_descriptor<remove_cv_t<T>>::value;
