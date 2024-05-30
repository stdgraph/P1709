template <class EL>
struct is_directed : public false_type {}; // specialized for graph container

template <class EL>
inline constexpr bool is_directed_v = is_directed<EL>::value;
