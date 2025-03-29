using namespace boost;

using G =
  compressed_sparse_row_graph<
     directedS, no_property, no_property>;
using VId = graph_traits<G>::vertex_descriptor;

G g;
//populate g

size_t count{0};
for(size_t i = 0; i < N; i++) {
  VId cur = vertex(i, g);
  count += num_triangles_on_vertex(g, cur);
}
count /= 6;
