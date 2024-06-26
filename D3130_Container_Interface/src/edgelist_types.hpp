template <basic_sourced_edgelist EL> // For exposition only
using edge_range_t = EL;

template <basic_sourced_edgelist EL> // For exposition only
using edge_iterator_t = ranges::iterator_t<edge_range_t<EL>>;

template <basic_sourced_edgelist EL> // For exposition only
using edge_t = ranges::range_value_t<edge_range_t<EL>>;

template <basic_sourced_edgelist EL> // For exposition only
using edge_reference_t = ranges::range_reference_t<edge_range_t<EL>>;

template <basic_sourced_edgelist EL> // For exposition only
using edge_value_t = decltype(edge_value(declval<edge_t<edge_range_t<EL>>>()));

template <basic_sourced_edgelist EL> // For exposition only
using vertex_id_t = decltype(source_id(declval<edge_t<edge_range_t<EL>>>()));
