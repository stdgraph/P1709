using namespace boost;

using G =
  compressed_sparse_row_graph<
     directedS, no_property, no_property>;

using edge_iterator = graph_traits<G>::out_edge_iterator;

size_t N(num_vertices(g));
size_t triangles(0);

for (size_t uid = 0; uid < N; ++uid) {
  Vertex u = vertex(uid, g);
  std::pair<edge_iterator, edge_iterator>
    u_neighbors = out_edges(u, g);

  auto i = u_neighbors.first;
  auto ie = u_neighbors.second;
  while (i < ie) {
    size_t vid = target(*i, g);
    Vertex v = vertex(vid, g);
    std::pair<edge_iterator, edge_iterator>
      v_neighbors = out_edges(v, g);

    auto i2 = i;
    auto j = v_neighbors.first;
    auto je = v_neighbors.second;

    while (i2 < ie && j < je) {
      size_t wid1 = target(*i2, g);
      size_t wid2 = target(*j, g);
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
