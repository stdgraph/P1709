using namespace std;
using namespace graph;

using G = container::compressed_graph<int, void, void, uint32_t, uint32_t>;

using VId = vertex_id_t<G>;

G g;
//populate g

vector<VId> p(size(vertices(g))); //predecessors
vector<int> d(size(vertices(g))); //distances
init_shortest_paths(distance, predecessors);
auto weight_fn = [&g](graph::edge_reference_t<graph_type> uv) -> int {
  return edge_value(g, uv);
};




dijkstra_shortest_paths(g, 0, d, p, weight_fn);
