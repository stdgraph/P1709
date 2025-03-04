using namespace std;
using namespace boost;

using G =
  compressed_sparse_row_graph<directedS, no_property, no_property>;

G g;
//populate g

vector<size_t> c(N); //components
size_t num = connected_components(g, &c[0]);
