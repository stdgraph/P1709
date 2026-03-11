# D3128r4 Algorithm Update — Implementation Plan

**Source of truth:** `algo_update_strategy.md`
**Target document:** `D3128_Algorithms/tex/algorithms.tex` and all `D3128_Algorithms/src/*.hpp`
**Reference implementation:** `graph-v3/include/graph/algorithm/`

All changes are applied to D3128r4 before publication. The implementation is the definitive
source; the LaTeX must conform to it.

---

## Approach

Changes are organized into six phases, ordered from lowest to highest risk:

1. **Decisions** — resolve open questions before touching files
2. **Syntax fixes** — mechanical corrections with no semantic ripple
3. **Shared concept files** — visitor and weight-function concepts that all algorithms depend on
4. **Algorithm signature files** — update each `src/*.hpp` signature file
5. **LaTeX prose** — update `algorithms.tex` text around signatures
6. **Additions and finalization** — new overloads, new algorithms, revision history

Each phase ends with a LaTeX build check: `cd P1709 && make D3128` (or equivalent).
Do not proceed to the next phase if the build fails.

---

## Phase 0 — Decisions (No File Edits)

These questions must be answered before Phase 4 so that the correct signature files are written.

### D1 — Tarjan's SCC
**Decision: (b) Keep with a "planned/future" note.**
Tarjan's SCC remains in the Tier 1 algorithm table but is marked as planned/future and
not yet implemented. A note in the specification should read:
> *Tarjan's strongly-connected-components algorithm is planned for a future revision.
> For current use, see `kosaraju` (`graph-v3/include/graph/algorithm/connected_components.hpp`),
> which provides both a two-graph overload and a single-graph overload for
> `index_bidirectional_adjacency_list` graphs.*

### D2 — `afforest`
**Decision: (a) Add to Tier 1.**
Add both overloads to the proposal alongside Connected Components:
- `afforest(G&&, Component&)` — single graph
- `afforest(G&&, GT&&, Component&)` — with transpose for bidirectional processing

> **Review note:** `afforest` is a less-familiar algorithm compared to the others in Tier 1.
> Before publication, verify that the specification is complete and that the WG21 audience
> has sufficient context (algorithm description, complexity, reference to Sutton et al. 2018).

### D3 — `inplace_kruskal`
**Decision: (a) Add alongside `kruskal`.**
Add both `inplace_kruskal` overloads (with and without comparator) to the MST section.
Source: `graph-v3/include/graph/algorithm/mst.hpp`.

### D4 — `_null_range_type` / `_null_predecessors`
**Decision: (b) Omit.** Keep the separate `_shortest_distances` overloads in the proposal
but leave the predecessor-elimination implementation detail invisible. No exposition-only
note is needed.

### D5 — `_id` visitor variants
**Decision: (a) Include in proposal.**
Add `has_on_initialize_vertex_id`, `has_on_discover_vertex_id`, `has_on_examine_vertex_id`,
`has_on_finish_vertex_id`, and `has_on_start_vertex_id` concepts alongside their descriptor
counterparts in `visitor_vertex.hpp`. These accept `vertex_id_t<G>` instead of `vertex_t<G>`
and are useful when the visitor only needs the ID (e.g., for graphs where vertex descriptors
are heavyweight or the ID alone is sufficient for bookkeeping).

---

## Phase 1 — Syntax Fixes

These are broken in the current `src/*.hpp` files and safe to fix in isolation.
No changes to `algorithms.tex` in this phase.

### 1.1 — `lp.hpp` syntax errors
**File:** `D3128_Algorithms/src/lp.hpp`
- Add missing comma between `Gen` and `class T` template parameters (line 16).
- Add missing comma after `empty_label` parameter (line 19).

### 1.2 — `mst.hpp` syntax error
**File:** `D3128_Algorithms/src/mst.hpp`
- Line 7: `template <index_edgelist_range IELR, index_edgelist_range OELR T, CompareOp>`
  is malformed. Fix: separate `OELR`, `class T`, and `class CompareOp` with commas.

### 1.3 — `breadth_first_search_multi.hpp` requires clause
**File:** `D3128_Algorithms/src/breadth_first_search_multi.hpp`
- Move `requires convertible_to<...>` to after the template parameter list,
  not on the same line as `void`.

### 1.4 — `topological_sort.hpp` requires clause
**File:** `D3128_Algorithms/src/topological_sort.hpp`
- Move `convertible_to<...>` constraint to after the template parameter list.

**Build check after Phase 1.**

---

## Phase 2 — Shared Concept Files

These files underpin all algorithms. Changing them here means algorithm signature files in
Phase 3 will automatically be consistent.

### 2.1 — `visitor_vertex.hpp` — replace `vertex_info` with `(g, descriptor)`
**File:** `D3128_Algorithms/src/visitor_vertex.hpp`

Replace every concept using the 1-arg `vertex_info<vertex_reference_t<G>, void>` form with
the 2-arg `(const G&, const vertex_t<G>&)` form. Apply to all five concepts:
`has_on_initialize_vertex`, `has_on_discover_vertex`, `has_on_start_vertex`,
`has_on_examine_vertex`, `has_on_finish_vertex`.

New form (example):
```cpp
template <class G, class Visitor>
concept has_on_initialize_vertex = // For exposition only
    requires(Visitor& v, const G& g, const vertex_t<G>& vdesc) {
      { v.on_initialize_vertex(g, vdesc) };
    };
```

If D5 is resolved to include `_id` variants, add them here as well.

### 2.2 — `visitor_edge.hpp` — replace `edge_info` with `(g, edge_t)`
**File:** `D3128_Algorithms/src/visitor_edge.hpp`

Replace every concept using the 1-arg `edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void>`
form with the 2-arg `(const G&, const edge_t<G>&)` form. Apply to all concepts:
`has_on_examine_edge`, `has_on_edge_relaxed`, `has_on_edge_not_relaxed`,
`has_on_edge_minimized`, `has_on_edge_not_minimized`,
`has_on_tree_edge`, `has_on_back_edge`, `has_on_forward_or_cross_edge`, `has_on_finish_edge`.

New form (example):
```cpp
template <class G, class Visitor>
concept has_on_examine_edge = // For exposition only
    requires(Visitor& v, const G& g, const edge_t<G>& e) {
      { v.on_examine_edge(g, e) };
    };
```

### 2.3 — `shortest_paths_helpers.hpp` — update weight concepts
**File:** `D3128_Algorithms/src/shortest_paths_helpers.hpp`

Three changes:

1. Add `edge_value_function` concept (prerequisite for the others):
   ```cpp
   template <class WF, class G, class E>
   concept edge_value_function = // For exposition only
       std::invocable<WF, const G&, E>;
   ```

2. Update `basic_edge_weight_function` to 2-arg form using `edge_t<G>` and
   `std::remove_reference_t<G>`:
   ```cpp
   template <class G, class WF, class DistanceValue, class Compare, class Combine>
   concept basic_edge_weight_function =
       edge_value_function<WF, std::remove_reference_t<G>, edge_t<G>> &&
       is_arithmetic_v<DistanceValue> && ...
   ```
   Add a comment explaining the `remove_reference_t<G>` pattern (G is a forwarding reference;
   for lvalue args G deduces as a reference type, so `const G&` would silently drop const).

3. Update `edge_weight_function` similarly to require 2-arg invocability.

**Build check after Phase 2.** The LaTeX should still compile with the updated concept
definitions (they are shown verbatim via `\lstinputlisting`).

---

## Phase 3 — Algorithm Signature Files

Update each `src/*.hpp` file to match the implementation. Work algorithm-by-algorithm;
do a build check at the end of each sub-group.

### 3.1 — Depth-First Search
**File:** `D3128_Algorithms/src/depth_first_search.hpp`

No signature change needed. Visitor concepts are already updated by Phase 2. Verify that
the `\lstinputlisting` reference compiles cleanly.

### 3.2 — Breadth-First Search
**Files:** `D3128_Algorithms/src/breadth_first_search.hpp`,
`D3128_Algorithms/src/breadth_first_search_multi.hpp`

- Visitor concepts updated by Phase 2; verify.
- `requires` clause position already fixed in Phase 1.

### 3.3 — Topological Sort (major rewrite)
**Files:** `D3128_Algorithms/src/topological_sort.hpp`,
`D3128_Algorithms/src/topological_sort_multi.hpp`,
`D3128_Algorithms/src/topological_sort_helpers.hpp`

Changes per `algo_update_strategy.md §2.3`:
1. Replace `Predecessors&` output parameter with `OutputIterator`.
2. Change return type from `void` to `bool` (`false` = cycle detected).
3. Add `[[nodiscard]]` attribute.
4. Add full-graph overload: `[[nodiscard]] bool topological_sort(const G&, OutputIterator)`.
5. Update `topological_sort_helpers.hpp`: remove or annotate `init_topological_sort` as removed.

Reference implementation: `graph-v3/include/graph/algorithm/topological_sort.hpp`.

### 3.4 — Dijkstra Shortest Paths and Distances
**Files:** `D3128_Algorithms/src/dijkstra_shortest_paths.hpp`,
`D3128_Algorithms/src/dijkstra_shortest_paths_multi.hpp`,
`D3128_Algorithms/src/dijkstra_shortest_dists.hpp`,
`D3128_Algorithms/src/dijkstra_shortest_dists_multi.hpp`

Changes per `§2.4–2.5`:
1. Replace 1-arg weight default `[](edge_reference_t<G> uv)` with
   `[](const auto&, const edge_t<G>& uv)`.
2. Change `source` parameter from `const vertex_id_t<G>` to `const vertex_id_t<G>&`.
3. `basic_edge_weight_function` concept is updated by Phase 2.3.

Reference: `graph-v3/include/graph/algorithm/dijkstra_shortest_paths.hpp`.

### 3.5 — Bellman-Ford Shortest Paths and Distances + `find_negative_cycle`
**Files:** `D3128_Algorithms/src/bellman_shortest_paths.hpp`,
`D3128_Algorithms/src/bellman_shortest_paths_multi.hpp`,
`D3128_Algorithms/src/bellman_shortest_dists.hpp`,
`D3128_Algorithms/src/bellman_shortest_dists_multi.hpp`,
`D3128_Algorithms/src/find_negative_cycle.hpp`

Changes per `§2.6–2.8`:
1. Same weight function and `source` parameter changes as Dijkstra (§3.4).
2. Add `[[nodiscard]]` to all four overloads.
3. `find_negative_cycle.hpp`: verify whether `G& g` should be `const G& g` (the algorithm
   only reads the graph — use `const G&`).

Reference: `graph-v3/include/graph/algorithm/bellman_ford_shortest_paths.hpp`.

### 3.6 — Triangle Counting
**File:** `D3128_Algorithms/src/tc.hpp`

Changes per `§2.9`:
1. Add `requires ordered_vertex_edges<G>` clause.
2. Add `[[nodiscard]]` and `noexcept`.

```cpp
template <index_adjacency_list G>
requires ordered_vertex_edges<G>
[[nodiscard]] size_t triangle_count(G&& g) noexcept;
```

### 3.7 — Label Propagation
**File:** `D3128_Algorithms/src/lp.hpp`

Changes per `§2.10` (syntax errors already fixed in Phase 1):
1. Add `requires equality_comparable<range_value_t<Label>> &&
         uniform_random_bit_generator<remove_cvref_t<Gen>>`.
2. Change `empty_label` parameter from `range_value_t<Label>&` to `range_value_t<Label>`
   (by value).

Reference: `graph-v3/include/graph/algorithm/label_propagation.hpp`.

### 3.8 — Maximal Independent Set
**File:** `D3128_Algorithms/src/mis.hpp`

Changes per `§2.16`:
1. Change return type from `void` to `size_t`.
2. Rename parameter `source` to `seed`.
3. Add default value `= 0` for `seed`.

```cpp
template <index_adjacency_list G, class Iter>
requires output_iterator<Iter, vertex_id_t<G>>
size_t maximal_independent_set(G&& g, Iter mis, vertex_id_t<G> seed = 0);
```

### 3.9 — Jaccard Coefficient
**File:** `D3128_Algorithms/src/jaccard.hpp`

Changes per `§2.17`:
1. Replace `requires is_invocable_v<OutOp, ...>` with `requires std::invocable<OutOp, ...>`.
2. Fix `OutOp` parameter types: remove `&` from vertex IDs; change `edge_reference_t<G>` to
   `edge_t<G>&`.

### 3.10 — Kruskal MST
**File:** `D3128_Algorithms/src/mst.hpp`

Changes per `§2.18` (syntax error already fixed in Phase 1):
1. Change return type from `void` to `pair<EV, size_t>` (total weight, number of components).
2. Update concept name: `index_edgelist_range` → `x_index_edgelist_range` (or whatever
   the final proposal name is — confirm against the implementation's edge list concepts).
3. If D3 resolves to include `inplace_kruskal`, add those overloads now.

Reference: `graph-v3/include/graph/algorithm/mst.hpp`.

### 3.11 — Prim MST
**File:** `D3128_Algorithms/src/prim.hpp` (also updated in `mst.hpp` for the proposal)

Changes per `§2.19`:
1. Change return type from `void` to the MST total weight type.
2. Add weight function parameter `WF` with default `[](const auto&, const edge_t<G>& uv)
   { return edge_value(g, uv); }`.
3. Rename parameter `source` to `seed`.
4. Add `requires basic_edge_weight_function<G, WF, ...>`.

Reference: `graph-v3/include/graph/algorithm/mst.hpp`.

### 3.12 — Connected Components, Articulation Points, Biconnected Components, Kosaraju
**File:** `D3128_Algorithms/src/connected_components.hpp`

This file currently contains all four algorithms. Changes per `§2.11–2.14`:

1. **Articulation Points** (lines 4–6): signature matches; no change.
2. **Biconnected Components** (lines 11–16): decide whether to keep or drop the
   `requires forward_range<...> && integral<...>` clause. The implementation has no
   explicit `requires` — confirm whether omitting it is intentional.
3. **Connected Components**: change return type from `void` to `size_t`.
4. **Kosaraju**: rename from `strongly_connected_components` to `kosaraju`;
   add the bidirectional overload `kosaraju(G&&, Component&)` that takes a single
   `index_bidirectional_adjacency_list G` graph and uses `in_edges`.

> Note: The presentation-only file `connected_components.hpp` may be split or left unified —
> the LaTeX can `\lstinputlisting` line ranges from a single file either way.

**Build check after Phase 3.** All signature files should typeset correctly.

---

## Phase 4 — LaTeX Prose (`algorithms.tex`)

After all `src/*.hpp` files are correct, update the surrounding LaTeX text in
`D3128_Algorithms/tex/algorithms.tex`. The signature listings are pulled in verbatim,
so this phase updates only: Complexity, Effects, Returns, Remarks, and section structure.

### 4.1 — BFS complexity
Correct the Complexity clause: remove the binary heap / O((E+V) log V) description and
replace with O(V+E) (FIFO queue).

### 4.2 — Topological Sort prose
1. Complexity: O(V+E) (DFS-based), not O((E+V) log V).
2. Returns clause: add description of `false` return on cycle detection.
3. Add specification section for the new full-graph overload `topological_sort(const G&, OutputIterator)`.
4. Remove or annotate the `init_topological_sort` helper description.

### 4.3 — Dijkstra error handling
Update the Hardened Preconditions / Throws discussion to reflect that the implementation
throws `std::out_of_range` with a `std::format` message. Reconcile with the hardened
preconditions approach chosen in r4.

### 4.4 — Bellman-Ford error handling
Same as §4.3 for Bellman-Ford.

### 4.5 — Connected Components return value
Update Returns clause: "Returns the number of connected components found."

### 4.6 — Kosaraju section
1. Rename section heading from "Strongly Connected Components (Kosaraju)" to `kosaraju`.
2. Add specification for the bidirectional overload `kosaraju(G&&, Component&)`.
3. Update or remove the Tarjan section per decision D1.

### 4.7 — MIS return and parameter prose
1. Returns clause: "Returns the number of vertices in the maximal independent set."
2. Update parameter description: `seed` (was `source`).

### 4.8 — Kruskal Returns clause
"Returns a `pair<EV, size_t>` containing the total weight of the spanning forest and
the number of connected components."

### 4.9 — Prim Returns clause and weight function
1. Returns clause: "Returns the total weight of the minimum spanning tree."
2. Add description of the `WF` weight function parameter.

### 4.10 — Triangle count concept note
Add a note or cross-reference explaining `ordered_vertex_edges<G>` (edges of each vertex
must be sorted in ascending order of `target_id`).

**Build check after Phase 4.**

---

## Phase 5 — Structural Additions

These tasks depend on the decisions from Phase 0 and the prose work from Phase 4.

### 5.1 — `ordered_vertex_edges` concept
Add a definition or reference for the `ordered_vertex_edges<G>` concept to the concepts
section of the document (or reference D3129).

### 5.2 — `afforest` specification (if D2 = add)
Add a full specification section for `afforest(G&&, Component&)` and
`afforest(G&&, GT&&, Component&)` alongside Connected Components.
Source: `graph-v3/include/graph/algorithm/connected_components.hpp`.

### 5.3 — `inplace_kruskal` specification (if D3 = add)
Add specification alongside `kruskal`. Source: `graph-v3/include/graph/algorithm/mst.hpp`.

### 5.4 — `_null_range_type` exposition note (if D4 = document)
Add an exposition-only note in the Dijkstra section explaining how
`dijkstra_shortest_distances` avoids predecessor overhead by passing `_null_predecessors`
to `dijkstra_shortest_paths`.

### 5.5 — `_id` visitor variant concepts (if D5 = include)
Add `has_on_initialize_vertex_id`, `has_on_discover_vertex_id`, etc. to `visitor_vertex.hpp`
and the concept section.

**Build check after Phase 5.**

---

## Phase 6 — Revision History and Cross-Paper Coordination

### 6.1 — `revision.tex` r4 entry
**File:** `D3128_Algorithms/tex/revision.tex`

Rewrite the r4 bullet list to accurately reflect what r4 contains. At minimum cover:
- Hardened preconditions (existing bullet, fix two typos: "defintions", "claus")
- Visitor concepts (vertex and edge) using 2-arg descriptor form
- `basic_edge_weight_function` and `edge_weight_function` concepts with 2-arg weight
- All new algorithms added in r4 (articulation points, biconnected components, connected
  components, Kosaraju, Tarjan/decision, MIS, Jaccard, label propagation, triangle count,
  Kruskal MST, Prim MST)
- Value-based descriptor design (`vertex_t<G>`, `edge_t<G>` replacing `vertex_reference_t`,
  `edge_reference_t`, `vertex_info`, `edge_info`)

### 6.2 — Cross-paper impacts
The reference → descriptor design shift (§0 of the strategy) is not isolated to D3128.
Identify and note the corresponding changes required in:
- **D3129 (Views)** — view iteration yields `vertex_data`/`edge_data` values; remove
  `vertex_info`/`edge_info` from view descriptions.
- **D3127 (Terminology/GCI)** — `vertex_t<G>`, `edge_t<G>` type aliases; remove
  `vertex_reference_t<G>` and `edge_reference_t<G>` if they are defined there.
- **D3130 (Container Interface)** — ensure container interface does not expose the old
  reference wrapper types.

Capture the specific sections of those papers that need updating in their respective
strategy/plan documents.

**Final build check: full clean build of D3128.**

---

## Status Table

Legend: `[ ]` = not started, `[~]` = in progress, `[x]` = complete, `[!]` = blocked / needs decision

### Phase 0 — Decisions

| ID  | Decision                                      | Status | Notes                                                                                         |
| --- | --------------------------------------------- | ------ | --------------------------------------------------------------------------------------------- |
| D1  | Tarjan SCC — keep, defer, or implement?       | `[x]`  | Option (b): keep with planned/future note; reference `kosaraju` in `connected_components.hpp` |
| D2  | `afforest` — add to Tier 1 or defer?          | `[x]`  | Option (a): add to Tier 1; review specification completeness before publication               |
| D3  | `inplace_kruskal` — add or omit?              | `[x]`  | Option (a): add both overloads alongside `kruskal`                                            |
| D4  | `_null_range_type` — exposition note or omit? | `[x]`  | Option (b): omit; implementation detail stays invisible                                       |
| D5  | `_id` visitor variants — include or omit?     | `[x]`  | Option (a): include all five `_id` vertex visitor concept variants                            |

### Phase 1 — Syntax Fixes

| ID   | Task                                      | File                                 | Status |
| ---- | ----------------------------------------- | ------------------------------------ | ------ |
| 1.1a | Fix comma between `Gen` and `class T`     | `src/lp.hpp`                         | `[ ]`  |
| 1.1b | Fix comma after `empty_label` param       | `src/lp.hpp`                         | `[ ]`  |
| 1.2  | Fix malformed Kruskal template param list | `src/mst.hpp`                        | `[ ]`  |
| 1.3  | Fix `requires` clause position            | `src/breadth_first_search_multi.hpp` | `[ ]`  |
| 1.4  | Fix `requires` clause position            | `src/topological_sort.hpp`           | `[ ]`  |
| —    | **Build check**                           |                                      | `[ ]`  |

### Phase 2 — Shared Concept Files

| ID  | Task                                                       | File                             | Status |
| --- | ---------------------------------------------------------- | -------------------------------- | ------ |
| 2.1 | Replace `vertex_info` → `(g, const vertex_t<G>&)`          | `src/visitor_vertex.hpp`         | `[ ]`  |
| 2.2 | Replace `edge_info` → `(g, const edge_t<G>&)`              | `src/visitor_edge.hpp`           | `[ ]`  |
| 2.3 | Update weight concepts to 2-arg; add `edge_value_function` | `src/shortest_paths_helpers.hpp` | `[ ]`  |
| —   | **Build check**                                            |                                  | `[ ]`  |

### Phase 3 — Algorithm Signature Files

| ID   | Task                                                                                            | File(s)                            | Status |
| ---- | ----------------------------------------------------------------------------------------------- | ---------------------------------- | ------ |
| 3.1  | Verify DFS (no changes expected)                                                                | `src/depth_first_search.hpp`       | `[ ]`  |
| 3.2  | Verify BFS (Phase 1+2 already fix it)                                                           | `src/breadth_first_search*.hpp`    | `[ ]`  |
| 3.3  | Topo sort: OutputIterator, bool, [[nodiscard]], full-graph                                      | `src/topological_sort*.hpp`        | `[ ]`  |
| 3.4  | Dijkstra: 2-arg weight, `source` → const ref                                                    | `src/dijkstra_shortest_*.hpp` (×4) | `[ ]`  |
| 3.5a | Bellman-Ford: 2-arg weight, `source` → const ref, [[nodiscard]]                                 | `src/bellman_shortest_*.hpp` (×4)  | `[ ]`  |
| 3.5b | `find_negative_cycle`: verify `const G&`                                                        | `src/find_negative_cycle.hpp`      | `[ ]`  |
| 3.6  | Triangle count: add `requires`, `[[nodiscard]]`, `noexcept`                                     | `src/tc.hpp`                       | `[ ]`  |
| 3.7  | Label propagation: add `requires`, fix `empty_label` type                                       | `src/lp.hpp`                       | `[ ]`  |
| 3.8  | MIS: `size_t` return, `seed` param, default `= 0`                                               | `src/mis.hpp`                      | `[ ]`  |
| 3.9  | Jaccard: `invocable`, fix param types                                                           | `src/jaccard.hpp`                  | `[ ]`  |
| 3.10 | Kruskal: return `pair<EV,size_t>`, fix concept name, add `inplace_kruskal` overloads            | `src/mst.hpp`                      | `[ ]`  |
| 3.11 | Prim: return weight, add WF, `seed`, `basic_edge_weight_function`                               | `src/prim.hpp`                     | `[ ]`  |
| 3.12 | Connected components: `size_t` return, kosaraju rename + bidir overload, biconn requires clause | `src/connected_components.hpp`     | `[ ]`  |
| —    | **Build check**                                                                                 |                                    | `[ ]`  |

### Phase 4 — LaTeX Prose

| ID   | Task                                                         | Section in `algorithms.tex`       | Status |
| ---- | ------------------------------------------------------------ | --------------------------------- | ------ |
| 4.1  | BFS complexity: O(V+E)                                       | BFS Complexity clause             | `[ ]`  |
| 4.2  | Topo sort: complexity + cycle detection + full-graph section | Topological Sort                  | `[ ]`  |
| 4.3  | Dijkstra: error handling reconciliation                      | Dijkstra Throws/Preconditions     | `[ ]`  |
| 4.4  | Bellman-Ford: error handling reconciliation                  | Bellman-Ford Throws/Preconditions | `[ ]`  |
| 4.5  | Connected components return description                      | Connected Components Returns      | `[ ]`  |
| 4.6  | Kosaraju: rename + bidir overload section + Tarjan (per D1)  | SCC section                       | `[!]`  |
| 4.7  | MIS returns + `seed` parameter description                   | MIS Returns/Parameters            | `[ ]`  |
| 4.8  | Kruskal Returns clause                                       | Kruskal Returns                   | `[ ]`  |
| 4.9  | Prim Returns clause + WF parameter description               | Prim Returns/Parameters           | `[ ]`  |
| 4.10 | Triangle count: `ordered_vertex_edges` note                  | Triangle Count Remarks            | `[ ]`  |
| —    | **Build check**                                              |                                   | `[ ]`  |

### Phase 5 — Structural Additions

| ID  | Task                                                        | Status | Depends on |
| --- | ----------------------------------------------------------- | ------ | ---------- |
| 5.1 | Add `ordered_vertex_edges` concept definition/reference     | `[ ]`  |            |
| 5.2 | Add `afforest` specification                                | `[!]`  | D2         |
| 5.3 | Add `inplace_kruskal` specification                         | `[!]`  | D3         |
| 5.4 | ~~Add `_null_range_type` exposition note~~ — omitted per D4 | `[x]`  | D4         |
| 5.5 | Add `_id` visitor variant concepts                          | `[ ]`  | D5         |
| —   | **Build check**                                             | `[ ]`  |            |

### Phase 6 — Revision History and Cross-Paper

| ID  | Task                                                                | File               | Status |
| --- | ------------------------------------------------------------------- | ------------------ | ------ |
| 6.1 | Rewrite r4 revision history entry (fix typos + add missing bullets) | `tex/revision.tex` | `[ ]`  |
| 6.2 | Document D3129 impacts (views / vertex_data / edge_data)            | (notes)            | `[ ]`  |
| 6.3 | Document D3127 impacts (terminology / vertex_t / edge_t)            | (notes)            | `[ ]`  |
| 6.4 | Document D3130 impacts (container interface)                        | (notes)            | `[ ]`  |
| —   | **Final clean build check**                                         |                    | `[ ]`  |
