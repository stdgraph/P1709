// For exposition only
namespace std::graph::edge_list {

template <basic_sourced_edgelist EL>
using edge_range_t = EL;

template <basic_sourced_edgelist EL>
using edge_iterator_t = ranges::iterator_t<edge_range_t<EL>>;

template <basic_sourced_edgelist EL>
using edge_t = ranges::range_value_t<edge_range_t<EL>>;

template <has_edge_value EL>
using edge_value_t =
    remove_cvref_t<decltype(edge_value(declval<edge_range_t<EL>&>(),
                                       declval<edge_t<edge_range_t<EL>>>()))>;

template <basic_sourced_edgelist EL>
using vertex_id_t =
    remove_cvref_t<decltype(source_id(declval<edge_range_t<EL>&>(),
                                      declval<edge_t<edge_range_t<EL>>>()))>;

template <basic_sourced_edgelist EL> // exposition only
using raw_vertex_id_t =
    decltype(source_id(declval<edge_range_t<EL>&>(),
                       declval<edge_t<edge_range_t<EL>>>()));

} // namespace std::graph::edge\_list
