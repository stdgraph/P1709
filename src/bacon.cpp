//
// Copyright 2021 Andrew Lumsdaine
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Simple example of finding bacon numbers from a given adjacency list graph
//

#include <iostream>
#include <tuple>
#include <string>
#include <vector>

#include "bfs_edge_range.hpp"

std::vector<std::string> actors { "Tom Cruise",       "Kevin Bacon",       "Hugo Weaving",
                                  "Carrie-Anne Moss", "Natalie Portman",   "Jack Nicholson",
                                  "Kelly McGillis",   "Harrison Ford",     "Sebastian Stan",
                                  "Mila Kunis",       "Michelle Pfeiffer", "Keanu Reeves",
                                  "Julia Roberts" };

using G = std::vector<std::vector<int>>;
auto target_id(const G& g, edge_reference_t<G> uv) {return get<0>(uv);}
G costar_adjacency_list{
    {1, 5, 6}, {7, 10, 0, 5, 12}, {4, 3, 11}, {2, 11}, {8, 9, 2, 12}, {0, 1}, {7, 0},
    {6, 1, 10}, {4, 9}, {4, 8}, {7, 1}, {2, 3}, {1, 4} };

int main() {
  std::vector<int> bacon_number(size(actors));

  // 1 -> Kevin Bacon
  for (auto&& [uid,vid] : basic_sourced_edges_bfs(costar_adjacency_list, 1)) { 
    bacon_number[vid] = bacon_number[uid] + 1;
  }

  for (int i = 0; i < size(actors); ++i) {
    std::cout << actors[i] << " has Bacon number " << bacon_number[i] << std::endl;
  }
}
