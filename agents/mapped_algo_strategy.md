# Mapped/Associated Container Algorithm Strategy

**Goal:** Discover all differences between the graph-v3 algorithm implementations (updated for
mapped/associated container support) and the D3128_Algorithms.tex paper, determine how to update
the paper, and identify related inconsistencies and implementation holes.

---

## §0  Background and Design Shift

The graph-v3 implementation has been extended so that most algorithms work with both:
- **Index graphs** — vertices are dense integer indices (`index_adjacency_list<G>`), backed by
  `std::vector` property maps.
- **Mapped graphs** — vertices are arbitrary hashable keys such as strings or structs
  (`mapped_adjacency_list<G>`), backed by `std::unordered_map` property maps.

The D3128 paper currently documents only the index-graph API.  All paper source files
(`D3128_Algorithms/src/*.hpp`) still use `index_adjacency_list` as the graph concept, and the paper
`tex/algorithms.tex` contains the editorial note *"Use `basic_index_adjacency_list` as the concept
for all algorithms"*  (line 12) — now outdated.

The new `graph-v3/include/graph/algorithm/*.hpp` files have been updated to use the generalised
`adjacency_list` concept together with a new `vertex_property_map_for` concept that abstracts over
vector and map-based property containers.  An `index/` subdirectory preserves the old
`index_adjacency_list` variants for reference.

---

## §1  New Infrastructure

### §1.1  `vertex_property_map<G,T>`

**Header:** `graph/adj_list/vertex_property_map.hpp`

A type alias that selects the appropriate container based on the graph type:

```cpp
// index_vertex_range graphs → vector
template <index_vertex_range G, class T>
using vertex_property_map = std::vector<T>;

// mapped graphs → unordered_map keyed on vertex_id_t<G>
template <mapped_vertex_range G, class T>
using vertex_property_map = std::unordered_map<vertex_id_t<G>, T>;
```

### §1.2  `vertex_property_map_for<M,G>` Concept

Defined at `vertex_property_map.hpp` line 214.  Requires that `M` supports subscript access by
`vertex_id_t<G>`:

```cpp
concept vertex_property_map_for = requires(M& m, const vertex_id_t<G>& uid) {
    m[uid];
};
```

Algorithms use this as the `requires` constraint instead of `random_access_range<Distances>` or
`std::vector<Distance>`.

### §1.3  `vertex_property_map_value_t<C>`

Extracts the stored value type:
- `std::vector<T>` → `T` (via `value_type`)
- `std::unordered_map<K,T>` → `T` (via `mapped_type`)

Used inside algorithm bodies as `using Distance = vertex_property_map_value_t<Distances>` instead
of a hardcoded template parameter.

### §1.4  Factory and Access Helpers

| Function | Description |
|---|---|
| `make_vertex_property_map<G,T>(g, init)` | Eager factory: `vector<T>(num_vertices(g), init)` _or_ `unordered_map` pre-reserved and filled |
| `make_vertex_property_map<G,T>(g)` | Lazy factory: sized vector _or_ empty reserved map |
| `vertex_property_map_contains(m, uid)` | `uid < size(m)` for vector; `.contains(uid)` for map |
| `vertex_property_map_get(m, uid, default_val)` | Read-with-fallback; never inserts into map |

### §1.5  New Graph Concepts

Defined in `graph/adj_list/adjacency_list_concepts.hpp` (lines ~320–400):

| Concept | Definition |
|---|---|
| `hashable_vertex_id<G>` | `std::hash<vertex_id_t<G>>` is valid |
| `mapped_vertex_range<G>` | `!index_vertex_range<G>` && `hashable_vertex_id<G>` && `find_vertex(g,uid)` valid |
| `mapped_adjacency_list<G>` | `adjacency_list<G>` && `mapped_vertex_range<G>` |
| `mapped_bidirectional_adjacency_list<G>` | `bidirectional_adjacency_list<G>` && `mapped_vertex_range<G>` |

---

## §2  Algorithm-by-Algorithm Changes

The table below compares the **current D3128 paper** (`D3128_Algorithms/src/`) with the
**graph-v3 implementation** (`graph-v3/include/graph/algorithm/`).

### §2.1  Traversal: BFS

| Item | Paper (src/breadth_first_search.hpp) | Implementation |
|---|---|---|
| Graph concept | `index_adjacency_list` | `adjacency_list` |
| Property maps | none (visitor-only) | internal `vertex_property_map<G,bool>` for visited |
| Source param | single `vertex_id_t<G>` | `input_range Sources` for multi-source |
| Validation | `size(visited) < num_vertices(g)` | `find_vertex(g, uid)` for mapped graphs |

### §2.2  Traversal: DFS

| Item | Paper | Implementation |
|---|---|---|
| Graph concept | `index_adjacency_list` | `adjacency_list` |
| Property maps | none | internal `vertex_property_map<G,bool>` for visited |

### §2.3  Traversal: Topological Sort

| Item | Paper | Implementation |
|---|---|---|
| Graph concept | `index_adjacency_list` | `adjacency_list` |
| All overloads | `index_adjacency_list` | `adjacency_list` |
| Output | `OutputIterator` of `vertex_id_t<G>` | same; `vertex_id_t<G>` works for mapped keys |

### §2.4  Shortest Paths: Dijkstra

| Item | Paper | Implementation |
|---|---|---|
| Graph concept | `index_adjacency_list` | `adjacency_list` |
| `Distances` param | `class Distances` requires `random_access_range<Distances>` | `class Distances` requires `vertex_property_map_for<Distances,G>` |
| `Predecessors` param | `class Predecessors` requires `random_access_range<Predecessors>` | same constraint updated to `vertex_property_map_for` |
| Distance type | `range_value_t<Distances>` | `vertex_property_map_value_t<Distances>` |
| Source validation | `size(distances) < num_vertices(g)` | branch on `index_vertex_range<G>`: size check vs `find_vertex` |
| PQ element type | `vertex_id_t<G>` | `vertex_t<G>` descriptor (avoids repeated find_vertex for mapped graphs) |

### §2.5  Shortest Paths: Bellman-Ford

| Item | Paper | Implementation |
|---|---|---|
| Graph concept | `index_adjacency_list` | `adjacency_list` |
| `Distances` / `Predecessors` | `random_access_range` requires | `vertex_property_map_for` requires |
| Distance type | `range_value_t<Distances>` | `vertex_property_map_value_t<Distances>` |
| `find_negative_cycle` | `index_adjacency_list` | `adjacency_list` |

### §2.6  MST: Prim

| Item | Paper | Implementation |
|---|---|---|
| Graph concept | `index_adjacency_list` | `adjacency_list` |
| `Predecessor` / `Weight` params | `random_access_range` | `vertex_property_map_for` |
| Prim paper file | `prim.hpp` (2 overloads) | same two overloads, concept updated |

### §2.7  MST: Kruskal

Paper `kruskal.hpp` uses `x_index_edgelist_range` — completely different (edge-list based, not
adjacency list).  Implementation `mst.hpp` has both Kruskal (edge list) and Prim (adjacency list).
No mapped-container change needed for Kruskal edge-list form.

### §2.8  Maximal Independent Set (MIS)

| Item | Paper | Implementation |
|---|---|---|
| Graph concept | `index_adjacency_list` | `adjacency_list` |
| Output | `class Iter` output iterator | same |
| Property map | — | internal `vertex_property_map<G,bool>` for `in_set` |

### §2.9  Triangle Counting (TC)

| Item | Paper | Implementation |
|---|---|---|
| Graph concept | `index_adjacency_list` | `adjacency_list` + `ordered_vertex_edges<G>` |
| Return type | `size_t` | `size_t` |
| No property maps needed | n/a | n/a |

### §2.10  Jaccard Coefficient

| Item | Paper | Implementation |
|---|---|---|
| Graph concept | `index_adjacency_list` | `adjacency_list` |
| Output | `OutOp` callback | same |

### §2.11  Label Propagation

| Item | Paper (`lp.hpp`) | Implementation (`label_propagation.hpp`) |
|---|---|---|
| Graph concept | `index_adjacency_list` | `adjacency_list` |
| `Label` param | `random_access_range` | `vertex_property_map_for<Label,G>` |

### §2.12  Connected Components (Undirected)

| Item | Paper | Implementation |
|---|---|---|
| Graph concept | `index_adjacency_list` | `adjacency_list` |
| `Component` param | `random_access_range<Component>` | `vertex_property_map_for<Component,G>` |

### §2.13  Articulation Points

| Item | Paper | Implementation |
|---|---|---|
| Graph concept | likely `index_adjacency_list` (no src file in paper) | `adjacency_list` |
| Output | output iterator of `vertex_id_t<G>` | same |

### §2.14  Biconnected Components

| Item | Paper | Implementation |
|---|---|---|
| Graph concept | likely `index_adjacency_list` (no src file in paper) | `adjacency_list` |
| Output | outer container of vertex sets | `OuterContainer` |

### §2.15  Kosaraju (Strongly Connected Components)

| Item | Paper (connected_components.hpp) | Implementation |
|---|---|---|
| Two-graph overload concept | `index_adjacency_list G, index_adjacency_list GT` | `adjacency_list G, adjacency_list GT` — **UPDATED** |
| Single-graph overload (bidirectional) | `index_bidirectional_adjacency_list G` | `index_bidirectional_adjacency_list G` — **NOT UPDATED** |
| `Component` param | `random_access_range<Component>` | `vertex_property_map_for<Component,G>` |

The single-graph Kosaraju overload (line 280 in implementation) still requires
`index_bidirectional_adjacency_list` — see §3.1 for analysis.

### §2.16  Afforest

Paper has `§Afforest Connected Components` section (line 1253 in algorithms.tex) but no
corresponding `D3128_Algorithms/src/*.hpp` file.  Status of mapped support unknown; not in
graph-v3 `algorithm/` directory either — **unimplemented**.

---

## §3  Implementation Holes and Inconsistencies

### §3.1  Kosaraju Single-Graph Overload — `index_bidirectional_adjacency_list`

**Location:** `graph-v3/include/graph/algorithm/connected_components.hpp` line 280.

The single-graph Kosaraju overload requires `index_bidirectional_adjacency_list<G>` and uses the
`in_edges()` view to perform the second DFS on the transpose without materialising it.  This is an
intentional constraint because:

1. Bidirectional (in-edge) support requires `mapped_bidirectional_adjacency_list<G>` for mapped
   graphs.
2. The concept `mapped_bidirectional_adjacency_list` **exists** (see §1.5), so upgrading to
   `bidirectional_adjacency_list<G>` (which covers both index-bidirectional and
   mapped-bidirectional) is technically feasible.

**Recommendation:** Investigate whether `mapped_bidirectional_adjacency_list` fully supports
`in_edges()` iterator.  If so, upgrade the single-graph Kosaraju overload to
`bidirectional_adjacency_list<G>` and add `vertex_property_map_for<Component,G>`.

### §3.2  Afforest — Unimplemented in graph-v3

Paper documents Afforest (§1.6 in algorithms.tex, §8 in algo_update_strategy.md) but it is absent
from `graph-v3/include/graph/algorithm/`.  No mapped-container variant exists.

### §3.3  Tarjan's SCC — No graph-v3 File

Paper has a stub for Tarjan's SCC (line 1389 in algorithms.tex) but no implementation.

### §3.4  Delta-Stepping — Paper Has Stub, No graph-v3 File

`D3128_Algorithms/src/delta_stepping.hpp` is effectively empty.  Not in `graph-v3/algorithm/`.

### §3.5  Stale Documentation Comments in Implementation

Several implementation files retain documentation mentioning `index_adjacency_list` requirements
(e.g., Bellman-Ford lines 86, 114) even though the template constraint has been updated to
`adjacency_list`.  These stale comments can mislead paper authors.

### §3.6  `has_negative_cycle` Return — Not a `vertex_property_map`

`bellman_ford_shortest_paths` returns a `bool` `has_negative_cycle` parameter by reference.  This
is not a property map; callers just pass a plain `bool`.  No change needed here.

### §3.7  Paper `random_access_range` `requires` Clauses

The paper uses `random_access_range<Predecessors>` and `random_access_range<Component>` in several
`requires` clauses (connected_components.hpp, mst.hpp, prim.hpp).  These are now
`vertex_property_map_for`, which is *weaker* — not all `vertex_property_map_for` maps are random
access.  Updating the concept name also changes the documented contract.

---

## §4  The `index/` Subdirectory

`graph-v3/include/graph/algorithm/index/` contains matched files for all 13 algorithm headers.
These use `index_adjacency_list` exclusively and represent the **old pre-mapped API**.

- The files in `D3128_Algorithms/src/` are conceptually the same as the `index/` variants, not the
  updated top-level files.
- When updating the paper, the `D3128_Algorithms/src/` files should be brought up to match the
  top-level `graph-v3/include/graph/algorithm/` files, not the `index/` variants.

The paper may eventually want to document both index and mapped forms, or only the general
`adjacency_list` form with a note that index graphs satisfy both.

---

## §5  D3128 Paper Gap Analysis

### §5.1  What the Paper Currently Shows

- Every algorithm section that has a `\lstinputlisting` pulls from `D3128_Algorithms/src/*.hpp`,
  all of which still use `index_adjacency_list`.
- There is no section covering `vertex_property_map`, `vertex_property_map_for`,
  `hashable_vertex_id`, `mapped_vertex_range`, or `mapped_adjacency_list`.
- The "Common Algorithm Definitions" section (§126 in algorithms.tex) covers edge weight concepts,
  ordered vertex edges, and visitor concepts — but not property maps.

### §5.2  Missing Paper Content

| Gap | Location in tex |
|---|---|
| `vertex_property_map<G,T>` type alias | New subsection needed in §Common Algorithm Definitions |
| `vertex_property_map_for<M,G>` concept | Same new subsection |
| `vertex_property_map_value_t<C>` | Same |
| Factory/access helpers | Same |
| `hashable_vertex_id<G>` / `mapped_vertex_range<G>` | Prerequisite concepts — possibly in D3130 Container Interface or in Common Defs |
| `mapped_adjacency_list<G>` / `mapped_bidirectional_adjacency_list<G>` | Same |

### §5.3  Concept Constraint Changes Per Section

| Paper Section | Function(s) | Old Constraint | New Constraint |
|---|---|---|---|
| BFS (§266) | `breadth_first_search` | `index_adjacency_list` | `adjacency_list` |
| DFS (§342) | `depth_first_search` | `index_adjacency_list` | `adjacency_list` |
| Topo sort (§408) | all overloads | `index_adjacency_list` | `adjacency_list` |
| Dijkstra (§528) | all overloads | `index_adjacency_list` + `random_access_range` | `adjacency_list` + `vertex_property_map_for` |
| Bellman-Ford (§705) | all overloads | same as Dijkstra | same change |
| TC (§925) | `triangle_count` | `index_adjacency_list` | `adjacency_list` |
| Label Propagation (§980) | `label_propagation` | `index_adjacency_list` + `random_access_range` | `adjacency_list` + `vertex_property_map_for` |
| Articulation Points (§1090) | `articulation_points` | likely `index_adjacency_list` | `adjacency_list` |
| Biconn. Cmpts (§1142) | `biconnected_components` | likely `index_adjacency_list` | `adjacency_list` |
| Conn. Cmpts (§1195) | `connected_components` | `index_adjacency_list` + `random_access_range` | `adjacency_list` + `vertex_property_map_for` |
| Kosaraju (§1325) | two-graph overload | `index_adjacency_list` | `adjacency_list` |
| Kosaraju (§1325) | single-graph (bidir) | `index_bidirectional_adjacency_list` | pending §3.1 resolution |
| MIS (§1394) | `maximal_independent_set` | `index_adjacency_list` | `adjacency_list` |
| Jaccard (§1449) | `jaccard_coefficient` | `index_adjacency_list` | `adjacency_list` |
| MST/Prim (§1504) | `prim` overloads | `index_adjacency_list` + `random_access_range` | `adjacency_list` + `vertex_property_map_for` |

---

## §6  Paper Update Plan

### Phase MA-0  Decisions

| ID | Decision | Options |
|---|---|---|
| D-MA-1 | **Scope of mapped support in paper**: document only the general `adjacency_list` form (subsumes index), or document both? | Recommended: general only, with a note that `index_adjacency_list` satisfies `adjacency_list` |
| D-MA-2 | **Where to place `vertex_property_map` section**: in D3128 "Common Definitions" or D3130 Container Interface? | Recommended: D3128 "Common Definitions" because it is tightly coupled to algorithm signatures |
| D-MA-3 | **`mapped_adjacency_list` / `hashable_vertex_id` concepts**: in D3128 or D3130? | Recommended: D3130 Container Interface (they describe container properties) with a cross-reference in D3128 |
| D-MA-4 | **Kosaraju single-graph overload**: upgrade to `bidirectional_adjacency_list` or keep as `index_bidirectional_adjacency_list`? | Investigate §3.1 first; prefer upgrade |
| D-MA-5 | **`Predecessors` type in Dijkstra/Bellman-Ford**: keep as separate template param or merge into `return` type? | Retain separate param matching implementation |

### Phase MA-1  Add `vertex_property_map` Section to Paper

**Location:** `D3128_Algorithms/tex/algorithms.tex`, between the current "Ordered Vertex Edges
Concept" subsection and the "Visitor Concepts" subsection (around line 196).

**Content to add:**
1. `vertex_property_map_for<M,G>` concept — definition, purpose, why it generalises both vector and
   map.
2. `vertex_property_map<G,T>` type alias.
3. `vertex_property_map_value_t<C>` value extractor.
4. `make_vertex_property_map` factory signatures.
5. `vertex_property_map_contains` / `vertex_property_map_get` access helpers.
6. Cross-reference to D3130 for `mapped_adjacency_list` concept definition.

Also update the stale editorial note at line 12:
- Remove or revise: *"Use `basic_index_adjacency_list` as the concept for all algorithms."*
- Replace with: *"Algorithms require `adjacency_list<G>` unless specifically constrained to
  `mapped_adjacency_list` or `index_adjacency_list`."*

### Phase MA-2  Update `D3128_Algorithms/src/*.hpp` Signature Files

For each file below, replace `index_adjacency_list` → `adjacency_list` and update `requires`
clauses for property map parameters:

| File | Change |
|---|---|
| `breadth_first_search.hpp` | `index_adjacency_list` → `adjacency_list` |
| `breadth_first_search_multi.hpp` | same |
| `depth_first_search.hpp` | same |
| `topological_sort.hpp` | same (both overloads) |
| `topological_sort_multi.hpp` | same |
| `dijkstra_shortest_paths.hpp` | `index_adjacency_list` → `adjacency_list`; `random_access_range<Distances>` → `vertex_property_map_for<Distances,G>` |
| `dijkstra_shortest_paths_multi.hpp` | same |
| `dijkstra_shortest_dists.hpp` | same |
| `dijkstra_shortest_dists_multi.hpp` | same |
| `bellman_shortest_paths.hpp` | same as Dijkstra |
| `bellman_shortest_paths_multi.hpp` | same |
| `bellman_shortest_dists.hpp` | same |
| `bellman_shortest_dists_multi.hpp` | same |
| `find_negative_cycle.hpp` | `index_adjacency_list` → `adjacency_list` |
| `tc.hpp` | `index_adjacency_list` → `adjacency_list`; add `ordered_vertex_edges<G>` |
| `jaccard.hpp` | `index_adjacency_list` → `adjacency_list` |
| `lp.hpp` (label propagation) | `index_adjacency_list` → `adjacency_list`; `random_access_range<Label>` → `vertex_property_map_for<Label,G>` |
| `connected_components.hpp` | `index_adjacency_list` → `adjacency_list`; `random_access_range<Component>` → `vertex_property_map_for<Component,G>` for applicable overloads |
| `mis.hpp` | `index_adjacency_list` → `adjacency_list` |
| `mst.hpp` | `index_adjacency_list` → `adjacency_list`; `random_access_range` → `vertex_property_map_for` for Predecessor/Weight |
| `prim.hpp` | same as `mst.hpp` |

Reference files: `graph-v3/include/graph/algorithm/*.hpp` (top-level, not `index/`).

### Phase MA-3  Update Algorithm Sections in `algorithms.tex`

For each section in §5.3:
1. Update the `\lstinputlisting` source (done automatically when src/*.hpp are updated in MA-2).
2. Update prose that mentions `index_adjacency_list` to `adjacency_list`.
3. Update `Preconditions` / `Postconditions` tables that reference `random_access_range` for
   property map parameters.
4. Cross-reference the new `vertex_property_map` subsection (from MA-1) in each algorithm's
   "Parameters" description for `Distances`, `Predecessors`, `Component`, `Label`, etc.

### Phase MA-4  Resolve Implementation Holes (see §3)

- **§3.1 Kosaraju bidir**: Check `mapped_bidirectional_adjacency_list` for `in_edges()` support.
  If available, upgrade the overload.  If not, document the limitation explicitly.
- **§3.2 Afforest**: Mark as "not yet specified" in the paper (already a stub).
- **§3.3 Tarjan's SCC**: Same — remain as future work.
- **§3.5 Stale doc comments**: Fix stale `index_adjacency_list` mentions in implementation
  docstrings (not strictly a paper task, but aid consistency review).

### Phase MA-5  Add Mapped Container Usage Examples

The algorithms.tex examples currently use index-based containers.  Add at least one mapped-graph
example demonstrating:
1. Constructing an `unordered_map`-based graph with string vertex IDs.
2. Creating distances/predecessors as `vertex_property_map<G,Distance>`.
3. Calling `dijkstra_shortest_paths` (or Bellman-Ford) with the mapped graph.
4. Reading results back via `vertex_property_map_get`.

---

## §7  Prioritised Task List

| Priority | Phase | Task | Notes |
|---|---|---|---|
| 1 | MA-0 | Resolve decisions D-MA-1 through D-MA-5 | Unblocks all other phases |
| 2 | MA-1 | Add `vertex_property_map` section to algorithms.tex | New prose, no src changes |
| 3 | MA-2 | Update `src/*.hpp` for BFS, DFS, topo sort | Concept-only change, simplest |
| 4 | MA-2 | Update `src/*.hpp` for Dijkstra and Bellman-Ford | Requires `vertex_property_map_for` in requires clauses |
| 5 | MA-2 | Update `src/*.hpp` for MST/Prim, Label Propagation, MIS | Similar pattern |
| 6 | MA-2 | Update `src/*.hpp` for TC, Jaccard, Connected Components | Includes §3.1 Kosaraju decision |
| 7 | MA-3 | Update prose in algorithms.tex per-algorithm sections | Can be done after MA-2 |
| 8 | MA-4 | Investigate and fix Kosaraju bidir hole | May require graph-v3 implementation work |
| 9 | MA-5 | Add mapped-container example | Lowest priority; needs MA-1 through MA-3 first |

---

## §8  Files Summary

| File | Role |
|---|---|
| `graph-v3/include/graph/adj_list/vertex_property_map.hpp` | New property map infrastructure (fully implemented) |
| `graph-v3/include/graph/adj_list/adjacency_list_concepts.hpp` | Mapped concept definitions (~lines 320–400) |
| `graph-v3/include/graph/algorithm/*.hpp` | Updated algorithm implementations (general `adjacency_list`) |
| `graph-v3/include/graph/algorithm/index/*.hpp` | Old `index_adjacency_list` variants preserved for reference |
| `D3128_Algorithms/src/*.hpp` | Paper signature snippets — all still use `index_adjacency_list` |
| `D3128_Algorithms/tex/algorithms.tex` | Paper body — no `vertex_property_map` content yet |
