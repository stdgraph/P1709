using namespace std;
using namespace boost;

using G =
  compressed_sparse_row_graph<
     directedS, no_property, no_property>;

G g;
//populate g

vector<size_t> c(num_vertices(g)); //components
size_t num_cmps = connected_components(g, &c[0]);
