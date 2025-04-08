template <index_adjacency_list G, input_range Sources, class Visitor = empty_visitor>
requires convertible_to<range_value_t<Sources>, vertex_id_t<G>> &&
void breadth_first_search(
      G&&            g,            // graph
      const Sources& sources,
      Visitor&&      visitor = empty_visitor())
