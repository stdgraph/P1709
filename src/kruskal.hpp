template <class VId, class EV, class E, class Iter>
requires std::output_iterator<Iter, std::tuple<VId, VId, EV>>
void kruskal(E&& e, Iter t) {}

template <class VId, class EV, class E, class Iter, class CompareOp>
requires std::output_iterator<Iter, std::tuple<VId, VId, EV>>
void kruskal(E&& e, Iter t, CompareOp compare) {}