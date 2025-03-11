using namespace std;
using namespace boost;

using G =
  compressed_sparse_row_graph<directedS, no_property, property<edge_weight_t, int>>;
using VId = graph_traits<G>::vertex_descriptor;

G g;
//populate g

vector<VId> p(num_vertices(g)); //predecessors
vector<int> d(num_vertices(g)); //distances

property_map< graph_t, edge_weight_t >::type weightmap = get(edge_weight, g);


dijkstra_shortest_paths(g, vertex(0, g), predecessor_map(make_iterator_property_map( p.begin(), get(vertex_index, g))).distance_map(make_iterator_property_map(d.begin(), get(vertex_index, g))));

