using namespace std;
using namespace graph;

using G = container::compressed_graph<int, void, void, uint32_t, uint32_t>;



G g;
//populate g


vector<int> d(size(vertices(g)), 0); //distances
auto weight_fn = [&g](graph::edge_reference_t<graph_type> uv) -> ValType {
  return edge_value(g, uv);
};



dijkstra_shortest_distances(g, 0, d, weight_fn);
