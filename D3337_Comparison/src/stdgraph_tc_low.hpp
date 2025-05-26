using namespace graph;

using G =
  container::compressed_graph<
     void, void, void, uint32_t, uint32_t>;




size_t N(size(vertices(g)));
size_t triangles(0);

for (vertex_id_t<G> uid = 0; uid < N; ++uid) {




  incidence_iterator<G> i(g, uid);
  auto                  ie = end(edges(g, uid));
  while (i != ie) {
    auto&& [vid, uv] = *i;
    



    incidence_iterator<G> j(g, vid);
    auto i2 = i;
    auto je = end(edges(g, vid));
    
    while (i2 != ie && j != je) {
      auto&& [wid1, uw] = *i2;
      auto&& [wid2, vw] = *j;
      if (wid1 < wid2) {
	++i2;
      } else if (wid2 < wid1) {
	++j;
      } else {
	++triangles;
	++i2;
	++j;
      }
    }
    ++i;
  }
}
