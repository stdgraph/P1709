using namespace std;
using namespace graph;

using G = container::compressed_graph<void, void, void, uint32_t, uint32_t>;
using VId = vertex_id_t<G>;


G g;
// populate g

vector<VId> parents(size(vertices(g));

auto bfs = edges_breadth_first_search_view<G,void,true>(g, 0);

for (auto&& [uid, vid, uv] : bfs) {
  parents[vid] = uid;
}
