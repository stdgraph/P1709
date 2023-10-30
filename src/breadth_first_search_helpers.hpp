template <property P, property D>
void init_breadth_first_search(P&& predecessors, D&& distances);

template <property P, property D>
void init_breadth_first_search(P&&                           predecessors,
                               D&&                           distances,
                               std::ranges::range_value_t<D> init);

template <property D>
void init_breadth_first_distances(D&& distances);

template <property D>
void init_breadth_first_distances(D&& distances, std::ranges::range_value_t<D> init);