using namespace std;
using namespace boost;

using G = compressed_sparse_row_graph<
             directedS, no_property, no_property>;
using Vertex = graph_traits<G>::vertex_descriptor;

G g;
//populate g

vector<Vertex> parents(num_vertices(g));

auto vis = make_bfs_visitor(
  make_pair(
    record_predecessors(parents.begin(),
                          on_tree_edge())));
breadth_first_search(g,
                       vertex(0, g),
                       visitor(vis));
