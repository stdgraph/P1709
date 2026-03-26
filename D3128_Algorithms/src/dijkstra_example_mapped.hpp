// Example: Dijkstra on a mapped graph (string vertex IDs, double edge weights)

#include <graph/container/dynamic_graph.hpp>
#include <graph/container/traits/uov_graph_traits.hpp>
#include <graph/adj_list/vertex_property_map.hpp>
#include <graph/algorithm/dijkstra_shortest_paths.hpp>
using namespace graph;
using namespace graph::adj_list;

using Traits = container::uov_graph_traits<double, void, void, std::string>;
using G      = container::dynamic_graph<double, void, void, std::string, false, Traits>;

G g({{"a", "b", 4.0}, {"a", "c", 2.0},
     {"b", "d", 5.0},
     {"c", "b", 1.0}, {"c", "d", 8.0}});

constexpr double inf = std::numeric_limits<double>::max();

std::unordered_map<std::string, double>      dist_map;
std::unordered_map<std::string, std::string> pred_map;
for (auto&& [uid, u] : views::vertexlist(g)) {
    dist_map[uid] = inf;
    pred_map[uid] = uid;
}

dijkstra_shortest_paths(g, std::string{"a"},
    [&dist_map](const auto&, const auto& uid) -> double& { return dist_map[uid]; },
    [&pred_map](const auto&, const auto& uid) -> std::string& { return pred_map[uid]; },
    [](const auto& g, const edge_t<G>& e) { return edge_value(g, e); });

// Shortest a to d: a to c (2) to b (1) to d (5) = 8
// dist\_map["d"] == 8

