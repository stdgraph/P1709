// For exposition only

template <class R, class G>
concept out_edge_range = forward_range<R> &&
                         edge<G, range_value_t<R>>;

template <class R, class G>
concept in_edge_range = forward_range<R> &&
                        edge<G, range_value_t<R>>;
