# D3128 Mapped Container Support — Implementation Plan

**Source of truth:** `mapped_algo_strategy.md`
**Target document:** `D3128_Algorithms/tex/algorithms.tex` and all `D3128_Algorithms/src/*.hpp`
**Reference implementation:** `graph-v3/include/graph/algorithm/` (top-level, **not** `index/`)

All changes bring the paper into alignment with the graph-v3 implementation's generalised
`adjacency_list` + `vertex_property_map_for` API.  The implementation is the definitive source;
the LaTeX must conform to it.

---

## Approach

Changes are organised into four phases, ordered from lowest to highest risk:

1. **Add vertex_property_map section** — new prose in "Common Algorithm Definitions" (paper only)
2. **Simple concept-only src updates** — `index_adjacency_list` → `adjacency_list`, no requires
   changes
3. **Requires-clause updates** — `random_access_range` → `vertex_property_map_for` plus related
   changes for Dijkstra, Bellman-Ford, MST/Prim, Label Propagation, Connected Components
4. **Prose updates and hole resolution** — algorithms.tex prose, Kosaraju bidir investigation,
   stale comments, examples

Each phase ends with a LaTeX build check: `cd /mnt/d/dev_graph/P1709 && make D3128`.
Do not proceed to the next phase if the build fails.

---

## Resolved Decisions

All design decisions were reviewed and confirmed before implementation began.

- **D-MA-1:** Document only the general `adjacency_list` form.  Note that every
  `index_adjacency_list<G>` automatically satisfies `adjacency_list<G>`; existing index-based
  callers require no changes.  No separate `index_adjacency_list` overloads.
- **D-MA-2:** `vertex_property_map` section goes in D3128 "Common Algorithm Definitions"
  (algorithms.tex §126), after "Ordered Vertex Edges Concept" and before "Visitor Concepts".
- **D-MA-3:** `hashable_vertex_id`, `mapped_vertex_range`, `mapped_adjacency_list`, and
  `mapped_bidirectional_adjacency_list` are defined in D3130; D3128 cross-references D3130.
- **D-MA-4:** Investigate `mapped_bidirectional_adjacency_list` + `in_edges()` support in
  Phase 4C; upgrade Kosaraju bidir overload if supported, otherwise document the limitation.
- **D-MA-5:** Retain `Predecessors` as a separate template parameter with
  `vertex_property_map_for` constraint, matching the implementation.

---

## Phase 1 — Add `vertex_property_map` Section to algorithms.tex

**File:** `D3128_Algorithms/tex/algorithms.tex`

Insert a new subsection "Vertex Property Map" between the closing of "Ordered Vertex Edges
Concept" (around line 195) and the opening of "Visitor Concepts and Classes" (around line 197).

### 1.1 — Remove or revise stale editorial note
**Location:** Line 12 of `algorithms.tex`.

Replace the `\phil{Use \tcode{basic_index_adjacency_list}...}` note with:
> *Algorithms require `adjacency_list<G>` unless the algorithm specifically constrains
> `mapped_adjacency_list<G>`, `index_adjacency_list<G>`, or a bidirectional variant.*

### 1.2 — Add `\subsection{Vertex Property Map}` content
New subsection content (in LaTeX, between §"Ordered Vertex Edges" and §"Visitor Concepts"):

1. Opening paragraph explaining why a property map abstraction is needed: algorithms work
   with both `std::vector<T>` (index graphs) and `std::unordered_map<vertex_id_t<G>,T>`
   (mapped graphs); callers choose the appropriate type; algorithms access both uniformly.

2. `vertex_property_map_for<M,G>` concept — show definition (lift from
   `vertex_property_map.hpp` line 214); explain it requires `m[uid]` subscript.

3. `vertex_property_map<G,T>` — show the conditional type alias (vector vs unordered_map).

4. `vertex_property_map_value_t<C>` — one-line description and definition.

5. Helper function signatures as a code listing:
   - `make_vertex_property_map<G,T>(g, init_value)` — eager factory
   - `make_vertex_property_map<G,T>(g)` — lazy factory
   - `vertex_property_map_contains(m, uid)` — presence test
   - `vertex_property_map_get(m, uid, default_value)` — read with fallback

6. Cross-reference: *"The concepts `mapped_adjacency_list<G>`, `hashable_vertex_id<G>`, and
   `mapped_vertex_range<G>` are defined in [D3130]."*

**Build check after Phase 1.**

---

## Phase 2 — Concept-Only `src/*.hpp` Updates

These files only need `index_adjacency_list` → `adjacency_list`; no `requires` clause changes.
All changes are mechanical and low-risk.

Work algorithm-by-algorithm.  For each file the reference is the corresponding
`graph-v3/include/graph/algorithm/*.hpp` (top-level).

### 2.1 — BFS
**Files:** `D3128_Algorithms/src/breadth_first_search.hpp`,
`D3128_Algorithms/src/breadth_first_search_multi.hpp`

- `template <index_adjacency_list G, ...>` → `template <adjacency_list G, ...>`
- No other signature changes needed.

### 2.2 — DFS
**File:** `D3128_Algorithms/src/depth_first_search.hpp`

- `template <index_adjacency_list G, ...>` → `template <adjacency_list G, ...>`

### 2.3 — Topological Sort
**Files:** `D3128_Algorithms/src/topological_sort.hpp`,
`D3128_Algorithms/src/topological_sort_multi.hpp`

- Both overloads: `index_adjacency_list` → `adjacency_list`.

### 2.4 — Triangle Counting
**File:** `D3128_Algorithms/src/tc.hpp`

- `index_adjacency_list` → `adjacency_list`.
- Add `ordered_vertex_edges<G>` to the `requires` clause (new constraint in implementation).

### 2.5 — Jaccard Coefficient
**File:** `D3128_Algorithms/src/jaccard.hpp`

- `index_adjacency_list` → `adjacency_list`.

### 2.6 — Maximal Independent Set
**File:** `D3128_Algorithms/src/mis.hpp`

- `index_adjacency_list` → `adjacency_list`.

### 2.7 — Find Negative Cycle
**File:** `D3128_Algorithms/src/find_negative_cycle.hpp`

- `index_adjacency_list` → `adjacency_list`.

**Build check after Phase 2.**

---

## Phase 3 — Requires-Clause Updates

These files require both the `index_adjacency_list` → `adjacency_list` change AND updating
property-map `requires` clauses from `random_access_range<X>` to `vertex_property_map_for<X,G>`.
Also update distance-type extraction from `range_value_t<X>` to `vertex_property_map_value_t<X>`.

### 3.1 — Dijkstra Shortest Paths
**Files:** `D3128_Algorithms/src/dijkstra_shortest_paths.hpp`,
`D3128_Algorithms/src/dijkstra_shortest_paths_multi.hpp`,
`D3128_Algorithms/src/dijkstra_shortest_dists.hpp`,
`D3128_Algorithms/src/dijkstra_shortest_dists_multi.hpp`

For each file:
- `template <index_adjacency_list G,` → `template <adjacency_list G,`
- `requires random_access_range<Distances>` → `requires vertex_property_map_for<Distances, G>`
- `requires random_access_range<Predecessors>` → `requires vertex_property_map_for<Predecessors, G>`
- Distance type extraction: `range_value_t<Distances>` → `vertex_property_map_value_t<Distances>`

Reference: `graph-v3/include/graph/algorithm/dijkstra_shortest_paths.hpp`

### 3.2 — Bellman-Ford Shortest Paths
**Files:** `D3128_Algorithms/src/bellman_shortest_paths.hpp`,
`D3128_Algorithms/src/bellman_shortest_paths_multi.hpp`,
`D3128_Algorithms/src/bellman_shortest_dists.hpp`,
`D3128_Algorithms/src/bellman_shortest_dists_multi.hpp`

Same pattern as §3.1.

Reference: `graph-v3/include/graph/algorithm/bellman_ford_shortest_paths.hpp`

### 3.3 — MST: Prim
**Files:** `D3128_Algorithms/src/prim.hpp`, `D3128_Algorithms/src/mst.hpp`

- `index_adjacency_list` → `adjacency_list`.
- `requires random_access_range<Predecessor>` → `requires vertex_property_map_for<Predecessor, G>`
- `requires random_access_range<Weight>` → `requires vertex_property_map_for<Weight, G>`

Reference: `graph-v3/include/graph/algorithm/mst.hpp` (Prim overloads)

Note: Kruskal overloads in `mst.hpp` / `kruskal.hpp` use `x_index_edgelist_range` — no change
needed.

### 3.4 — Label Propagation
**File:** `D3128_Algorithms/src/lp.hpp`

- `index_adjacency_list` → `adjacency_list` (both overloads).
- `requires random_access_range<Label>` (or equivalent) → `requires vertex_property_map_for<Label, G>`

Reference: `graph-v3/include/graph/algorithm/label_propagation.hpp`

### 3.5 — Connected Components (Undirected)
**File:** `D3128_Algorithms/src/connected_components.hpp`

Four overloads (`Iter`, `OuterContainer`, and two `Component` forms):
- All `index_adjacency_list` → `adjacency_list` where applicable.
- `requires random_access_range<Component>` → `requires vertex_property_map_for<Component, G>`
  on the overloads that take a `Component` out-parameter.

Kosaraju two-graph overload (`index_adjacency_list G, adjacency_list GT` in paper):
- Update both `G` and `GT` to `adjacency_list`.
- Update `Component` constraint to `vertex_property_map_for<Component, G>`.

Kosaraju single-graph overload (bidirectional): leave at `index_bidirectional_adjacency_list`
pending Phase 4C investigation.

Reference: `graph-v3/include/graph/algorithm/connected_components.hpp`

**Build check after Phase 3.**

---

## Phase 4 — Prose Updates and Hole Resolution

### 4A — Update algorithm prose in `algorithms.tex`
**File:** `D3128_Algorithms/tex/algorithms.tex`

For each of the 15 algorithm sections listed in `mapped_algo_strategy.md §5.3`:
1. The `\lstinputlisting` content updates automatically (src/*.hpp were already changed in
   Phases 2–3).
2. Update inline prose that mentions `index_adjacency_list` — replace with `adjacency_list`.
3. Update "Parameters" / "Preconditions" / "Postconditions" tables for `Distances`,
   `Predecessors`, `Component`, `Label`, `Predecessor`, `Weight` parameters: change type
   description from *"random-access range"* to *"vertex property map satisfying
   `vertex_property_map_for<M,G>`"* and add a cross-reference to the §vertex_property_map
   subsection added in Phase 1.
4. Add a note under each affected algorithm's "Type requirements" row:
   *"For index graphs, a `std::vector` satisfies `vertex_property_map_for`.
   For mapped graphs, use `std::unordered_map<vertex_id_t<G>, T>` or
   `vertex_property_map<G, T>`."*

Algorithms that need prose updates:
- BFS, DFS, Topological Sort
- Dijkstra (all four overloads), Bellman-Ford (all four overloads)
- Triangle Counting, Jaccard
- Label Propagation
- Articulation Points, Biconnected Components
- Connected Components (undirected)
- Kosaraju two-graph overload
- MIS
- MST / Prim

### 4B — Stale implementation comments
**Scope:** `graph-v3/include/graph/algorithm/` (not the paper, but aids review consistency).

Files confirmed to have stale `index_adjacency_list` references in docstring prose (not in
template constraints):
- `bellman_ford_shortest_paths.hpp` lines 86, 114 — mention "G must satisfy
  `index_adjacency_list`" in comments that post-date the constraint change.

For each: Update the comment to read "`adjacency_list`" (or "`adjacency_list` (index or mapped)").
These are not paper files; make a separate commit labelled as implementation comment cleanup.

### 4C — Kosaraju single-graph (bidirectional) overload investigation
**Location:** `graph-v3/include/graph/algorithm/connected_components.hpp` line 280.

Steps:
1. Check whether `mapped_bidirectional_adjacency_list<G>` supports `in_edges(g, *ui)` where
   `ui` is a mapped vertex iterator.  Look at `graph-v3/include/graph/adj_list/` for the
   `in_edges` view implementation over mapped containers.
2. **If supported:** upgrade the template constraint from
   `index_bidirectional_adjacency_list<G>` to `bidirectional_adjacency_list<G>` in both
   the implementation file and `D3128_Algorithms/src/connected_components.hpp`.
   Update the `Component` constraint to `vertex_property_map_for<Component,G>`.
3. **If not supported:** keep `index_bidirectional_adjacency_list<G>`, and add a prose note
   in `algorithms.tex` Kosaraju section:
   > *"The single-graph overload requires `index_bidirectional_adjacency_list<G>` because
   > mapped bidirectional containers do not currently expose `in_edges()`.  Use the
   > two-graph overload (providing an explicit transpose) with a mapped graph."*
4. Update `D3128_Algorithms/src/connected_components.hpp` to match whichever outcome above.

**Build check after Phase 4.**

---

## Phase 4D — Mapped Container Example

**File:** `D3128_Algorithms/tex/algorithms.tex` or a new `D3128_Algorithms/src/` example file.

Add one new code example demonstrating end-to-end mapped-graph algorithm usage.  The example
should appear in the Dijkstra section (§528) as a companion to any existing index-graph example.

Example outline (C++ code listing):
```cpp
// Graph with string vertex IDs (mapped container)
using G = /* mapped adjacency_list type */;
G g;
// ... add vertices "a", "b", "c" and edges ...

vertex_property_map<G, double> dist = make_vertex_property_map<G, double>(g, inf);
vertex_property_map<G, vertex_id_t<G>> pred = make_vertex_property_map<G, vertex_id_t<G>>(g, "");

dijkstra_shortest_paths(g, "a"sv, dist, pred,
    [](const G& g, const edge_t<G>& e) { return edge_value(g, e); });

// Read result
std::cout << vertex_property_map_get(dist, "c"sv, inf) << "\n";
```

The concrete mapped container type should be derived from an actual graph-v3 mapped container
used in the existing test suite (check `graph-v3/tests/` for usage).

**Build check after Phase 4.**

---

## Checklist Summary

| Phase | Task | Status |
|---|---|---|
| — | D-MA-1: document `adjacency_list` form only | confirmed |
| — | D-MA-2: `vertex_property_map` section → D3128 Common Defs | confirmed |
| — | D-MA-3: mapped graph concepts → D3130, cross-ref in D3128 | confirmed |
| — | D-MA-4: Kosaraju bidir — investigate in Phase 4C | confirmed |
| — | D-MA-5: retain `Predecessors` as separate param | confirmed |
| 1 | Revise stale editorial note (line 12) | not started |
| 1 | Add `\subsection{Vertex Property Map}` to algorithms.tex | not started |
| 1 | Build check | not started |
| 2 | BFS concept change | not started |
| 2 | DFS concept change | not started |
| 2 | Topological Sort concept change | not started |
| 2 | Triangle Counting concept + `ordered_vertex_edges` | not started |
| 2 | Jaccard concept change | not started |
| 2 | MIS concept change | not started |
| 2 | Find Negative Cycle concept change | not started |
| 2 | Build check | not started |
| 3 | Dijkstra — concept + requires + value_t (4 files) | not started |
| 3 | Bellman-Ford — concept + requires + value_t (4 files) | not started |
| 3 | Prim/MST — concept + requires (2 files) | not started |
| 3 | Label Propagation — concept + requires | not started |
| 3 | Connected Components — concept + requires (incl. Kosaraju two-graph) | not started |
| 3 | Build check | not started |
| 4A | Prose updates in algorithms.tex (15 sections) | not started |
| 4B | Stale comments in graph-v3 implementation | not started |
| 4C | Kosaraju bidir investigation and fix/note | not started |
| 4 | Build check | not started |
| 4D | Mapped-graph Dijkstra example | not started |
| 4 | Build check (final) | not started |

---

## Files Reference

| File | Phase | Role |
|---|---|---|
| `D3128_Algorithms/tex/algorithms.tex` | 1, 4A, 4D | New subsection; prose updates; example |
| `D3128_Algorithms/src/breadth_first_search.hpp` | 2 | Concept only |
| `D3128_Algorithms/src/breadth_first_search_multi.hpp` | 2 | Concept only |
| `D3128_Algorithms/src/depth_first_search.hpp` | 2 | Concept only |
| `D3128_Algorithms/src/topological_sort.hpp` | 2 | Concept only |
| `D3128_Algorithms/src/topological_sort_multi.hpp` | 2 | Concept only |
| `D3128_Algorithms/src/tc.hpp` | 2 | Concept + add `ordered_vertex_edges` |
| `D3128_Algorithms/src/jaccard.hpp` | 2 | Concept only |
| `D3128_Algorithms/src/mis.hpp` | 2 | Concept only |
| `D3128_Algorithms/src/find_negative_cycle.hpp` | 2 | Concept only |
| `D3128_Algorithms/src/dijkstra_shortest_paths.hpp` | 3 | Concept + requires + value_t |
| `D3128_Algorithms/src/dijkstra_shortest_paths_multi.hpp` | 3 | Concept + requires + value_t |
| `D3128_Algorithms/src/dijkstra_shortest_dists.hpp` | 3 | Concept + requires + value_t |
| `D3128_Algorithms/src/dijkstra_shortest_dists_multi.hpp` | 3 | Concept + requires + value_t |
| `D3128_Algorithms/src/bellman_shortest_paths.hpp` | 3 | Concept + requires + value_t |
| `D3128_Algorithms/src/bellman_shortest_paths_multi.hpp` | 3 | Concept + requires + value_t |
| `D3128_Algorithms/src/bellman_shortest_dists.hpp` | 3 | Concept + requires + value_t |
| `D3128_Algorithms/src/bellman_shortest_dists_multi.hpp` | 3 | Concept + requires + value_t |
| `D3128_Algorithms/src/prim.hpp` | 3 | Concept + requires |
| `D3128_Algorithms/src/mst.hpp` | 3 | Concept + requires (Prim overloads only) |
| `D3128_Algorithms/src/lp.hpp` | 3 | Concept + requires |
| `D3128_Algorithms/src/connected_components.hpp` | 3, 4C | Concept + requires; Kosaraju bidir TBD |
| `graph-v3/.../bellman_ford_shortest_paths.hpp` | 4B | Stale docstring comments only |
| `graph-v3/.../connected_components.hpp` | 4C | Kosaraju bidir overload (if upgraded) |
