template <class VId, class EV, class E, class Iter>
requires output_iterator<Iter, tuple<VId, VId, EV>>
void kruskal(E&& e, Iter t);

template <class VId, class EV, class E, class Iter, class CompareOp>
requires output_iterator<Iter, tuple<VId, VId, EV>>
void kruskal(E&& e, Iter t, CompareOp compare);
