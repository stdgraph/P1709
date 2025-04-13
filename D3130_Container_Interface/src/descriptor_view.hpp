template <forward_range R>
constexpr auto descriptor_view(R&& r);

template <forward_range R>
using descriptor_view_t = decltype(descriptor_view(declval<R>()));

template <forward_range R>
constexpr auto descriptor_subrange_view(R&& rng, R&& subrng);

template <forward_range R>
using descriptor_subrange_view_t = decltype(descriptor_subrange_view(declval<R>(), declval<R>()));
