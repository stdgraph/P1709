using namespace std;
using namespace graph;

using G = container::compressed_graph<void, void, void, uint32_t, uint32_t>;


G g;
//populate g

vector<size_t> component(size(vertices(g)));

connected_components(g, component);
size_t num = ranges::max_element(component) + 1;




