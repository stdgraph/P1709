using namespace std;
using namespace graph;

using G = container::compressed_graph<void, void, void, uint32_t, uint32_t>;


G g;
//populate g

vector<size_t> c(size(vertices(g))); //components
size_t num = connected_components(g, c);




