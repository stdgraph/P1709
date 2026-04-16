template <adjacency_list G, class Visitor = empty_visitor,
          class Alloc = allocator<byte>>
void depth_first_search(
      G&&             g,            // graph
      vertex_id_t<G>  source,       // starting vertex\_id
      Visitor&&       visitor = empty_visitor(),
      const Alloc&    alloc   = Alloc())
