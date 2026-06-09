// For exposition only
namespace std::graph::edge_list {

template <class EL>
concept basic_sourced_edgelist =
    ranges::input_range<EL> &&
    !ranges::range<ranges::range_value_t<EL>> &&
    basic_edge<EL, ranges::range_value_t<EL>>;   // shared edge floor: source\_id + target\_id

template <class EL>
concept basic_sourced_index_edgelist =
    basic_sourced_edgelist<EL> &&
    requires(EL& el, ranges::range_value_t<EL> uv) {
      { source_id(el, uv) } -> integral;
      { target_id(el, uv) } -> integral;
    };

template <class EL>
concept has_edge_value =
    basic_sourced_edgelist<EL> &&
    requires(EL& el, ranges::range_value_t<EL> uv) {
      { edge_value(el, uv) };
    };

} // namespace std::graph::edge\_list
