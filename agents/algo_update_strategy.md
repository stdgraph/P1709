# Algorithm Document Update Strategy

## Purpose

This document captures all discrepancies between the **D3128 Algorithms** LaTeX proposal
(D3128r4, unreleased) and the **graph-v3** reference implementation in
`graph-v3/include/graph/algorithm/`, along with a concrete plan to bring the LaTeX
documents into alignment with the implementation. Since D3128r4 has not yet been released,
all updates described here are applied directly to D3128r4 before publication.

**The implementation is the definitive/authoritative source.** Where the LaTeX and the
implementation disagree, the implementation is correct and the LaTeX must be updated.

---

## 0. Fundamental Design Shift: References → Value-Based Descriptors

The most significant change between the LaTeX proposal and the current implementation is a
**fundamental design shift from reference-based types to value-based descriptors**. This
affects the entire API surface — visitors, weight functions, algorithm parameters, return
types, and view iteration.

### 0.1 Old Design (LaTeX / D3128r4)

The LaTeX uses **reference-based wrapper types**:

- `vertex_reference_t<G>` — a reference to a vertex in the graph's internal storage
- `edge_reference_t<G>` — a reference to an edge in the graph's internal storage
- `vertex_info<vertex_reference_t<G>, void>` — a wrapper struct bundling a vertex reference
- `edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void>` — a wrapper struct bundling
  source_id, target_id, and an edge reference

These types are **not in the implementation**. `vertex_reference_t<G>` and
`edge_reference_t<G>` do not exist in graph-v3.

### 0.2 New Design (Implementation)

The implementation uses **value-based descriptors** accessed through type aliases:

- `vertex_t<G>` = `range_value_t<vertex_range_t<G>>` — a vertex descriptor (value type)
- `edge_t<G>` = `range_value_t<out_edge_range_t<G>>` — an edge descriptor (value type)
- `vertex_id_t<G>` — the vertex identifier type (typically integral)

These are **not references** — they are the `value_type` of the graph's vertex and edge
ranges. The graph's ranges yield descriptors by value, not references into internal storage.

The old wrapper types have been **renamed and redesigned**:

| Old (LaTeX)                                                  | New (Implementation)             | Notes                                                                          |
| ------------------------------------------------------------ | -------------------------------- | ------------------------------------------------------------------------------ |
| `vertex_info<vertex_reference_t<G>, void>`                   | `vertex_data<VId, V, VV>`        | Value type with fields `{id, vertex, value}` + void specializations            |
| `edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void>` | `edge_data<VId, Sourced, E, EV>` | Value type with fields `{source_id, target_id, edge, value}` + specializations |
| N/A                                                          | `copyable_vertex_t<VId, VV>`     | Alias for `vertex_data<VId, void, VV>` — `{id, value}`                         |
| N/A                                                          | `copyable_edge_t<VId, EV>`       | Alias for `edge_data<VId, true, void, EV>` — `{source_id, target_id, value}`   |
| N/A                                                          | `edgelist_edge<VId, E, EV>`      | Alias for `edge_data<VId, true, E, EV>`                                        |

`vertex_data` has **8 template specializations** (void combinations of VId, V, VV).
`edge_data` has **16 template specializations** (void combinations of VId, Sourced, E, EV).

These types are defined in `graph_data.hpp` and used by views for structured bindings:
```cpp
for (auto&& [uid, u]        : vertexlist(g))        // vertex_data<VId, V, void>
for (auto&& [uid, u, value] : vertexlist(g, vvf))   // vertex_data<VId, V, VV>
for (auto&& [vid, uv]       : incidence(g, u))       // edge_data<VId, false, E, void>
for (auto&& [uid, vid, uv]  : sourced_incidence(...)) // edge_data<VId, true, E, void>
```

### 0.3 Impact on Algorithms

This design change is **pervasive** — it affects:

1. **All visitor concepts** — changed from 1-arg `(wrapper)` to 2-arg `(graph, descriptor)` form
2. **All weight functions** — changed from 1-arg `weight(edge_ref)` to 2-arg `weight(graph, edge_descriptor)`
3. **Algorithm signatures** — use `vertex_t<G>`, `edge_t<G>`, `vertex_id_t<G>` instead of reference types
4. **Return types** — algorithms return `vertex_data`/`edge_data` values, not references
5. **View iteration** — views yield descriptor values, enabling structured bindings

**Action:** This is the single most important update for D3128. Every reference to
`vertex_reference_t<G>`, `edge_reference_t<G>`, `vertex_info`, or `edge_info` in the LaTeX
must be replaced with the corresponding descriptor/data types. This change also affects
D3129 (Views) and D3127 (GCI) — coordinate updates across papers.

---

## 1. Global / Cross-Cutting Changes

### 1.1 Visitor Concept Signatures — `vertex_info`/`edge_info` → `(graph, descriptor)` Pairs

| Aspect                  | LaTeX (D3128r4)                                                                               | Implementation (authoritative)                                                                                             |
| ----------------------- | --------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------- |
| Vertex visitor concepts | `v.on_X(vdesc)` where `vdesc` is `vertex_info<vertex_reference_t<G>, void>`                   | `v.on_X(g, vdesc)` where `g` is `const G&` and `vdesc` is `const vertex_t<G>&` (also `_id` variants with `vertex_id_t<G>`) |
| Edge visitor concepts   | `v.on_X(edesc)` where `edesc` is `edge_info<vertex_id_t<G>, true, edge_reference_t<G>, void>` | `v.on_X(g, e)` where `g` is `const G&` and `e` is `const edge_t<G>&`                                                       |

The old LaTeX wrapper types (`vertex_info`, `edge_info`) have been replaced by the value-based
descriptor design described in §0. Visitors now receive the graph object alongside the descriptor
so they can query the graph (e.g., look up vertex values, adjacent edges) without capturing it.

**Action:** Replace all `visitor_vertex.hpp` and `visitor_edge.hpp` in `D3128_Algorithms/src/`
with the implementation's two-argument form: `(const G&, descriptor)`. Remove all references to
`vertex_info`, `edge_info`, `vertex_reference_t<G>`, and `edge_reference_t<G>` from the
algorithms document. The implementation also provides `_id` concept variants
(e.g., `has_on_initialize_vertex_id`) that accept `vertex_id_t<G>` — decide whether these
should be in the proposal.

### 1.2 Weight Function Arity — `weight(uv)` → `weight(g, uv)`

| Aspect                         | LaTeX (D3128r4)                                   | Implementation (authoritative)                                        |
| ------------------------------ | ------------------------------------------------- | --------------------------------------------------------------------- |
| Default weight function        | `[](edge_reference_t<G> uv) { return 1; }`        | `[](const auto&, const edge_t<G>& uv) { return 1; }`                  |
| Weight concept                 | `WF(edge_reference_t<G>)` (1-arg)                 | `WF(const remove_reference_t<G>&, edge_t<G>)` (2-arg)                 |
| `basic_edge_weight_function`   | 1-arg: `invoke_result_t<WF, edge_reference_t<G>>` | 2-arg: `invoke_result_t<WF, const remove_reference_t<G>&, edge_t<G>>` |
| `edge_weight_function` concept | Same issue                                        | Same issue                                                            |

Note: `edge_reference_t<G>` does not exist in the implementation; it has been replaced by
`edge_t<G>` (a value-based descriptor). See §0 for the full design shift details.

**Action:** Update `basic_edge_weight_function` and `edge_weight_function` concepts in
`algorithms.tex` to use the 2-arg weight function form with `edge_t<G>` instead of
`edge_reference_t<G>`. Update all Dijkstra and Bellman-Ford signature source files to match.
Also incorporate the `edge_value_function` concept used in the implementation and the
`std::remove_reference_t<G>` pattern (with an explanatory note).

### 1.3 `_null_range_type` / `_null_predecessors` Pattern

The implementation uses a `_null_range_type` sentinel (exposition-only) to eliminate
predecessor tracking overhead at compile-time in the `*_shortest_distances()` overloads.
The shortest-distances functions delegate to shortest-paths with `_null_predecessors`.

**Action:** If the proposal wants to explain how `dijkstra_shortest_distances` avoids
predecessor overhead, add a brief exposition-only note about this pattern. Otherwise, no
change needed — but ensure the separate overload signatures are retained.

### 1.4 `vertex_id_store_t<G>` type alias

The implementation introduces `vertex_id_store_t<G>` (exposition-only in `traversal_common.hpp`)
with a `static_assert` verifying it matches `vertex_id_t<G>` for `index_adjacency_list`. This
type alias doesn't appear in the LaTeX.

**Action:** Decide whether to include this as exposition-only in the proposal.

### 1.5 `empty_visitor` definition

| Aspect     | LaTeX (D3128r4)            | Implementation |
| ---------- | -------------------------- | -------------- |
| Definition | `struct empty_visitor {};` | Same           |

No change needed — these match.

### 1.6 Complexity Clause Format

All algorithm Complexity clauses in the proposal must specify **both time and space complexity**.
The implementation headers are the authoritative source for these values (Doxygen comments in
`graph-v3/include/graph/algorithm/*.hpp`). Where the proposal states only one dimension,
add the missing one. Where a value is wrong, correct it.

Standard format for every Complexity clause:
- **Time:** O(…)
- **Space:** O(…) auxiliary (excluding graph storage and caller-supplied output parameters)

Complexity values from the reference implementation:

| Algorithm                | Time               | Space (auxiliary)                             |
| ------------------------ | ------------------ | --------------------------------------------- |
| BFS                      | O(V+E)             | O(V) — visited array + queue                  |
| DFS                      | O(V+E)             | O(V) — color array + DFS stack                |
| Topological sort         | O(V+E)             | O(V) — color array + finish order + DFS stack |
| Dijkstra                 | O((V+E) log V)     | O(V) — priority queue + bookkeeping           |
| Bellman-Ford             | O(V·E)             | O(1)                                          |
| Triangle count           | O(m^{3/2}) avg     | O(1)                                          |
| Label propagation        | O(M) per iteration | O(V) — shuffled IDs + frequency map           |
| MIS                      | O(V+E)             | O(V) — removed-vertex tracking array          |
| Jaccard                  | O(V + E·d_min)     | O(V+E) — precomputed neighbor sets            |
| Kruskal                  | O(E log E)         | O(E+V) — edge copy + union-find               |
| Inplace Kruskal          | O(E log E)         | O(V) — union-find only (no edge copy)         |
| Prim                     | O(E log V)         | O(V) — priority queue + arrays                |
| Connected components     | O(V+E)             | O(V) — component array + DFS stack            |
| Kosaraju (2-graph)       | O(V+E)             | O(V) — visited array + finish order           |
| Kosaraju (bidirectional) | O(V+E)             | O(V) — visited array + finish order           |
| afforest                 | O(V + E·α(V))      | O(V) — component array only                   |

**Action:** Cross-reference this table when writing or reviewing Phase 4 prose. Every
algorithm section must include both Time and Space rows in its Complexity clause.

---

## 2. Per-Algorithm Discrepancies

### 2.1 Breadth-First Search

| Aspect                     | LaTeX (D3128r4) `breadth_first_search.hpp`                                                                                             | Implementation `breadth_first_search.hpp`                                                                                              |
| -------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------- |
| Multi-source signature     | `requires convertible_to<...> && void breadth_first_search(...)`                                                                       | `void breadth_first_search(G&&, const Sources&, Visitor&&)` — same shape                                                               |
| `requires` clause position | Before return type (awkward placement)                                                                                                 | After template params                                                                                                                  |
| Complexity                 | O((E+V) log V) — mentions binary heap                                                                                                  | O(V+E) — BFS uses FIFO queue, not priority queue                                                                                       |
| Remarks                    | "dijkstra provides extended functionality"                                                                                             | Same concept present                                                                                                                   |
| BFS visitor events listed  | `on_initialize_vertex, on_discover_vertex, on_examine_vertex, on_finish_vertex, on_examine_edge, on_edge_relaxed, on_edge_not_relaxed` | `on_initialize_vertex, on_discover_vertex, on_examine_vertex, on_finish_vertex, on_examine_edge, on_edge_relaxed, on_edge_not_relaxed` |

**Action:**
1. **Fix complexity**: BFS uses a FIFO queue, not a priority queue. Change time from O((E+V) log V) to **O(V+E)**; add space complexity **O(V)** (visited array + queue). See §1.6 for the full reference table.
2. Update visitor concept signatures (see §1.1).
3. Update weight function arity if BFS uses one (it does not in the current implementation — BFS is
   unweighted, visitor-only).
4. Update the `requires` clause formatting so it is after the template parameter list.

### 2.2 Depth-First Search

| Aspect         | LaTeX (D3128r4)                                                                                                                                                      | Implementation                                   |
| -------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------ |
| Signature      | Single-source only                                                                                                                                                   | Single-source only                               |
| Visitor events | `on_initialize_vertex, on_start_vertex, on_discover_vertex, on_finish_vertex, on_examine_edge, on_tree_edge, on_back_edge, on_forward_or_cross_edge, on_finish_edge` | Same set (all present in `traversal_common.hpp`) |
| Multi-source   | Commented out in LaTeX                                                                                                                                               | Not in implementation either                     |

**Action:**
1. Update visitor concept signatures per §1.1.
2. Otherwise largely aligned. Minor formatting cleanup.

### 2.3 Topological Sort

| Aspect             | LaTeX (D3128r4)                                                  | Implementation                                                                         |
| ------------------ | ---------------------------------------------------------------- | -------------------------------------------------------------------------------------- |
| Interface          | Takes `Predecessors&` range; returns `void`                      | Takes `OutputIterator`; returns `bool` (false if cycle detected)                       |
| Single-source      | `void topological_sort(const G&, vertex_id_t<G>, Predecessors&)` | `[[nodiscard]] bool topological_sort(const G&, const vertex_id_t<G>&, OutputIterator)` |
| Multi-source       | `void topological_sort(const G&, const Sources&, Predecessors&)` | `[[nodiscard]] bool topological_sort(const G&, const Sources&, OutputIterator)`        |
| Full-graph variant | **Not in LaTeX**                                                 | `bool topological_sort(const G&, OutputIterator)` — processes all vertices             |
| Helpers            | `init_topological_sort(predecessors)` with `iota`                | No separate helper; initialization is internal                                         |
| Cycle detection    | Not mentioned                                                    | Returns `false` when a back edge is detected                                           |
| Complexity         | O((E+V) log V) — incorrectly mentions priority queue             | O(V+E) — DFS-based                                                                     |

**Action:**
1. **Major rewrite**: Replace `Predecessors&` output with `OutputIterator` to match implementation.
2. **Change return type** from `void` to `bool` to reflect cycle detection capability.
3. **Add `[[nodiscard]]` attribute**.
4. **Add full-graph variant** `topological_sort(const G&, OutputIterator)`.
5. **Remove `init_topological_sort` helper** or mark it as removed.
6. **Fix complexity**: time from O((E+V) log V) to **O(V+E)** (DFS-based); add space complexity **O(V)** (color array + finish order + DFS stack). See §1.6.
7. **Add cycle detection description** to Effects/Returns.
8. Update `D3128_Algorithms/src/topological_sort.hpp`, `topological_sort_multi.hpp`,
   and `topological_sort_helpers.hpp`.

### 2.4 Dijkstra Shortest Paths

| Aspect             | LaTeX (D3128r4)                                           | Implementation (authoritative)                                                 |
| ------------------ | --------------------------------------------------------- | ------------------------------------------------------------------------------ |
| Weight function    | 1-arg: `weight(uv)`                                       | 2-arg: `weight(g, uv)`                                                         |
| Weight default     | `[](edge_reference_t<G> uv) { return 1; }`                | `[](const auto&, const edge_t<G>& uv) { return 1; }`                           |
| `source` type      | `const vertex_id_t<G>` (by value)                         | `const vertex_id_t<G>&` (by const ref)                                         |
| `constexpr`        | Yes                                                       | Yes                                                                            |
| Error handling     | Not discussed (Hardened Preconditions)                    | Throws `std::out_of_range` with `std::format` message                          |
| Overload structure | 4 separate files (paths, paths_multi, dists, dists_multi) | 4 overloads in single file, dists delegates to paths with `_null_predecessors` |

**Action:**
1. Update weight function to 2-arg form (see §1.2).
2. Change `source` parameter to `const vertex_id_t<G>&` (by const ref).
3. Update `basic_edge_weight_function` concept.
4. Update all 4 source files in `D3128_Algorithms/src/`.
5. Consider whether to show the delegation pattern (dists → paths with null predecessors)
   or keep them separate.

### 2.5 Dijkstra Shortest Distances

Same changes as §2.4 Dijkstra Shortest Paths.

### 2.6 Bellman-Ford Shortest Paths

| Aspect                     | LaTeX (D3128r4)                                                                                   | Implementation                         |
| -------------------------- | ------------------------------------------------------------------------------------------------- | -------------------------------------- |
| Function name in signature | `bellman_ford_shortest_paths`                                                                     | `bellman_ford_shortest_paths`          |
| Weight function            | 1-arg                                                                                             | 2-arg (same as Dijkstra)               |
| `source` type              | `const vertex_id_t<G>` (by value)                                                                 | `const vertex_id_t<G>&` (by const ref) |
| `[[nodiscard]]`            | Not present                                                                                       | Present on all overloads               |
| Error handling             | Hardened preconditions                                                                            | `std::out_of_range` with `std::format` |
| Visitor events             | `on_examine_edge, on_edge_relaxed, on_edge_not_relaxed, on_edge_minimized, on_edge_not_minimized` | Same set                               |

**Action:**
1. Same weight function update as Dijkstra (§1.2).
2. Add `[[nodiscard]]` to all Bellman-Ford signatures.
3. Change `source` parameter to `const vertex_id_t<G>&`.
4. Update all 4 source files.

### 2.7 Bellman-Ford Shortest Distances

Same changes as §2.6 Bellman-Ford Shortest Paths.

### 2.8 `find_negative_cycle`

| Aspect      | LaTeX (D3128r4) | Implementation (authoritative)                                            |
| ----------- | --------------- | ------------------------------------------------------------------------- |
| Graph param | `const G& g`    | `G& g` (non-const — may be a bug in implementation, or const was dropped) |

**Action:** Verify whether `g` should be `const G&` or `G&` and reconcile. The algorithm only reads
the graph, so `const G&` is appropriate.

### 2.9 Triangle Counting

| Aspect              | LaTeX (D3128r4)                                                    | Implementation                                                         |
| ------------------- | ------------------------------------------------------------------ | ---------------------------------------------------------------------- |
| Signature           | `template <index_adjacency_list G>` `size_t triangle_count(G&& g)` | Same, plus `requires ordered_vertex_edges<G>` clause                   |
| `[[nodiscard]]`     | Not present                                                        | Present                                                                |
| `noexcept`          | Not present                                                        | Present                                                                |
| Concept requirement | None shown                                                         | `ordered_vertex_edges<G>` required (edges must be sorted by target_id) |

**Action:**
1. Add `requires ordered_vertex_edges<G>` to LaTeX signature.
2. Add `[[nodiscard]]` and `noexcept`.
3. Add the `ordered_vertex_edges` concept definition (from implementation) to the concepts section
   or reference it.

### 2.10 Label Propagation

| Aspect                | LaTeX (D3128r4) `lp.hpp`                                                                     | Implementation `label_propagation.hpp`                                                              |
| --------------------- | -------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------- |
| File name             | `lp.hpp`                                                                                     | `label_propagation.hpp`                                                                             |
| Additional requires   | None                                                                                         | `equality_comparable<range_value_t<Label>>` and `uniform_random_bit_generator<remove_cvref_t<Gen>>` |
| empty_label overload  | `empty_label` is `range_value_t<Label>&` (by reference)                                      | `empty_label` is `range_value_t<Label>` (by value)                                                  |
| Syntax error in LaTeX | Missing comma after `empty_label` parameter; missing comma between Gen and T template params | N/A                                                                                                 |

**Action:**
1. Fix syntax errors in `lp.hpp` (missing commas).
2. Add `requires` clause with `equality_comparable` and `uniform_random_bit_generator`.
3. Change `empty_label` parameter from `range_value_t<Label>&` to `range_value_t<Label>` (by value).
4. Consider renaming source file from `lp.hpp` to `label_propagation.hpp` for consistency.

### 2.11 Articulation Points

| Aspect                | LaTeX (D3128r4)                                      | Implementation                           |
| --------------------- | ---------------------------------------------------- | ---------------------------------------- |
| Source file inclusion | Lines 4–6 of `connected_components.hpp`              | Separate file: `articulation_points.hpp` |
| Signature             | `void articulation_points(G&& g, Iter cut_vertices)` | Same                                     |
| `requires` clause     | `requires output_iterator<Iter, vertex_id_t<G>>`     | Same                                     |

**Action:**
1. Consider updating the `\lstinputlisting` to use `articulation_points.hpp` source or
   keep it in `connected_components.hpp` for the proposal — but track that the
   implementation now has a separate file.
2. The signature itself matches.

### 2.12 Biconnected Components

| Aspect            | LaTeX (D3128r4)                                                                                                       | Implementation                                  |
| ----------------- | --------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------- |
| Source file       | Lines 11–16 of `connected_components.hpp`                                                                             | Separate file: `biconnected_components.hpp`     |
| `requires` clause | `requires forward_range<range_value_t<OuterContainer>> && integral<forward_range_t<forward_range_t<OuterContainer>>>` | No explicit `requires` clause in implementation |

**Action:**
1. Update `\lstinputlisting` source reference.
2. Verify if `requires` clause is needed or if the implementation relies on SFINAE/concepts
   internally. The implementation template has no requires clause — just `template <index_adjacency_list G, class OuterContainer>`.
3. Consider adding back the old `requires` clause or dropping it for simplicity.

### 2.13 Connected Components

| Aspect      | LaTeX (D3128r4)                              | Implementation                                 |
| ----------- | -------------------------------------------- | ---------------------------------------------- |
| Return type | `void`                                       | `size_t` (returns number of components)        |
| Signature   | `void connected_components(G&&, Component&)` | `size_t connected_components(G&&, Component&)` |

**Action:**
1. **Change return type** from `void` to `size_t`.
2. Add Returns clause: "Returns the number of connected components found."
3. Update source file.

### 2.14 Kosaraju's Strongly Connected Components

| Aspect        | LaTeX (D3128r4)                         | Implementation                                                                                           |
| ------------- | --------------------------------------- | -------------------------------------------------------------------------------------------------------- |
| Function name | `strongly_connected_components`         | `kosaraju`                                                                                               |
| Overloads     | One overload: `(G&&, GT&&, Component&)` | Two overloads: `(G&&, GT&&, Component&)` and `(G&&, Component&)` for bidirectional graphs                |
| GT concept    | `index_adjacency_list GT`               | `index_adjacency_list GT` (same for 2-graph); `index_bidirectional_adjacency_list G` for 1-graph variant |

**Action:**
1. **Rename function** in LaTeX from `strongly_connected_components` to `kosaraju`.
2. **Add the bidirectional overload** `kosaraju(G&&, Component&)` that uses `in_edges` instead
   of a separate transpose graph.
3. Consider the naming convention: the implementation uses the algorithm name (`kosaraju`)
   rather than the generic name (`strongly_connected_components`).

### 2.15 Tarjan's Strongly Connected Components

| Aspect   | LaTeX (D3128r4)                                                                            | Implementation      |
| -------- | ------------------------------------------------------------------------------------------ | ------------------- |
| Presence | Included as `strongly_connected_components(G&&, Component&)` with `adjacency_list` concept | **Not implemented** |
| Concept  | `adjacency_list G` (less restrictive)                                                      | N/A                 |

**Action:**
1. Decide whether to keep Tarjan's SCC in the proposal. It is not currently implemented in
   graph-v3. Options:
   - Remove from the Tier 1 list.
   - Mark as planned/future.
   - Implement it in graph-v3 first.
2. If kept, note that the Kosaraju bidirectional overload partially replaces this use case.

### 2.16 Maximal Independent Set

| Aspect                 | LaTeX (D3128r4) | Implementation                              |
| ---------------------- | --------------- | ------------------------------------------- |
| Return type            | `void`          | `size_t` (returns count of vertices in MIS) |
| `seed` parameter       | `source`        | `seed`                                      |
| Default value for seed | None            | `= 0`                                       |

**Action:**
1. **Change return type** from `void` to `size_t`.
2. **Rename parameter** from `source` to `seed` (or decide on unified naming).
3. **Add default value** `= 0` for seed.
4. Update Returns clause: "Returns the number of vertices in the maximal independent set."

### 2.17 Jaccard Coefficient

| Aspect                | LaTeX (D3128r4)                                            | Implementation (authoritative)                  |
| --------------------- | ---------------------------------------------------------- | ----------------------------------------------- |
| `requires` clause     | `requires is_invocable_v<OutOp, ...>`                      | `requires std::invocable<OutOp, ...>`           |
| OutOp parameter types | `vertex_id_t<G>&, vertex_id_t<G>&, edge_reference_t<G>, T` | `vertex_id_t<G>, vertex_id_t<G>, edge_t<G>&, T` |

Note: `edge_reference_t<G>` has been replaced by `edge_t<G>` (value-based descriptor). See §0.

**Action:**
1. Change `is_invocable_v` to `std::invocable` (concept, not type trait).
2. Adjust parameter types: remove `&` from vertex IDs (pass by value), change
   `edge_reference_t<G>` to `edge_t<G>&` (descriptor by reference).

### 2.18 Kruskal's MST

| Aspect       | LaTeX (D3128r4)                                                                                 | Implementation                                             |
| ------------ | ----------------------------------------------------------------------------------------------- | ---------------------------------------------------------- |
| Concept      | `index_edgelist_range`                                                                          | `x_index_edgelist_range`                                   |
| Overloads    | 2 (with/without compare)                                                                        | 4: `kruskal` × 2 + `inplace_kruskal` × 2                   |
| Return type  | `void`                                                                                          | `auto` → `pair<EV, size_t>` (total weight, num components) |
| Syntax error | Missing comma in `template <index_edgelist_range IELR, index_edgelist_range OELR T, CompareOp>` | N/A                                                        |

**Action:**
1. Fix syntax error (missing comma between `OELR` and `T` — should be separate template params
   or `CompareOp` should be a template param, not `T`).
2. Add `inplace_kruskal` variants.
3. Change return type from `void` to `pair<EV, size_t>`.
4. Update concept name `index_edgelist_range` → `x_index_edgelist_range` (or decide on
   final name for the proposal).
5. Add Returns clause.

### 2.19 Prim's MST

| Aspect                       | LaTeX (D3128r4)                               | Implementation                                                               |
| ---------------------------- | --------------------------------------------- | ---------------------------------------------------------------------------- |
| Return type                  | `void`                                        | `auto` → `EV` (total weight of MST)                                          |
| Weight function              | Not present (uses `edge_value_t<G>` directly) | Accepts `WF` weight function parameter; default uses `edge_value(g, uv)` CPO |
| Parameter name               | `source`                                      | `seed`                                                                       |
| `compare` parameter          | Edge value comparator                         | Same concept, but `WF` also present                                          |
| Error handling               | Mandates clause                               | `std::out_of_range` exceptions                                               |
| `basic_edge_weight_function` | Not in requires                               | Required in implementation                                                   |

**Action:**
1. **Change return type** from `void` to weight type (total MST weight).
2. **Add weight function parameter** `WF` with default using `edge_value` CPO.
3. **Rename parameter** from `source` to `seed`.
4. Add `basic_edge_weight_function` requirement.
5. Add Returns clause.

---

## 3. New Algorithms in Implementation Not in LaTeX

### 3.1 `afforest` (Connected Components)

The implementation includes `afforest()` — a parallel-friendly connected components algorithm
using neighbor sampling (Sutton et al., 2018). Two overloads:
- `afforest(G&&, Component&)` — single graph
- `afforest(G&&, GT&&, Component&)` — with transpose for bidirectional processing

**Action:** Decide whether to add `afforest` to Tier 1 algorithms or defer to Tier 2.

### 3.2 `inplace_kruskal`

Memory-efficient variant that sorts the input edge list in place. See §2.18.

### 3.3 Full-Graph `topological_sort`

`topological_sort(const G&, OutputIterator)` — processes all vertices, not just reachable from sources.
See §2.3.

---

## 4. Algorithms in LaTeX Not Implemented

### 4.1 Tarjan's Strongly Connected Components

In the LaTeX as `strongly_connected_components(G&&, Component&)` with `adjacency_list` concept.
**Not implemented** in graph-v3. See §2.15.

---

## 5. Source File Reorganization

The LaTeX document includes source files from `D3128_Algorithms/src/` via `\lstinputlisting`.
The implementation has reorganized files:

| LaTeX Source File(s)                                                                                                                                          | Implementation File                                                                            |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------- |
| `breadth_first_search.hpp`, `breadth_first_search_multi.hpp`                                                                                                  | `breadth_first_search.hpp` (both in one file)                                                  |
| `depth_first_search.hpp`                                                                                                                                      | `depth_first_search.hpp`                                                                       |
| `topological_sort.hpp`, `topological_sort_multi.hpp`, `topological_sort_helpers.hpp`                                                                          | `topological_sort.hpp` (all in one file)                                                       |
| `dijkstra_shortest_paths.hpp`, `dijkstra_shortest_paths_multi.hpp`, `dijkstra_shortest_dists.hpp`, `dijkstra_shortest_dists_multi.hpp`                        | `dijkstra_shortest_paths.hpp` (all in one file)                                                |
| `bellman_shortest_paths.hpp`, `bellman_shortest_paths_multi.hpp`, `bellman_shortest_dists.hpp`, `bellman_shortest_dists_multi.hpp`, `find_negative_cycle.hpp` | `bellman_ford_shortest_paths.hpp` (all in one file)                                            |
| `shortest_paths_helpers.hpp`                                                                                                                                  | `traversal_common.hpp`                                                                         |
| `visitor_vertex.hpp`, `visitor_edge.hpp`, `visitor_other.hpp`                                                                                                 | `traversal_common.hpp`                                                                         |
| `connected_components.hpp` (all component algos)                                                                                                              | Split into `articulation_points.hpp`, `biconnected_components.hpp`, `connected_components.hpp` |
| `tc.hpp`                                                                                                                                                      | `tc.hpp`                                                                                       |
| `lp.hpp`                                                                                                                                                      | `label_propagation.hpp`                                                                        |
| `mis.hpp`                                                                                                                                                     | `mis.hpp`                                                                                      |
| `jaccard.hpp`                                                                                                                                                 | `jaccard.hpp`                                                                                  |
| `mst.hpp`                                                                                                                                                     | `mst.hpp`                                                                                      |

**Action:** The LaTeX source files in `D3128_Algorithms/src/` should be updated to match the
implementation signatures. The LaTeX can continue to split signatures into separate files for
readability — the file split is presentation-only.

---

## 6. Documentation Cross-References

The `graph-v3/docs/user-guide/algorithms/` directory contains comprehensive markdown
documentation for all implemented algorithms. These docs are **more current** than the LaTeX
and serve as a good reference for:

| Topic                                        | User-Guide Doc              |
| -------------------------------------------- | --------------------------- |
| Articulation Points                          | `articulation_points.md`    |
| Bellman-Ford                                 | `bellman_ford.md`           |
| BFS                                          | `bfs.md`                    |
| Biconnected Components                       | `biconnected_components.md` |
| Connected Components (+ Kosaraju + Afforest) | `connected_components.md`   |
| DFS                                          | `dfs.md`                    |
| Dijkstra                                     | `dijkstra.md`               |
| Jaccard                                      | `jaccard.md`                |
| Label Propagation                            | `label_propagation.md`      |
| MIS                                          | `mis.md`                    |
| MST (Kruskal + Prim)                         | `mst.md`                    |
| Topological Sort                             | `topological_sort.md`       |
| Triangle Count                               | `triangle_count.md`         |

**Action:** Use these docs as ground truth when updating LaTeX signatures and descriptions.
The implementation is the definitive source; these docs reflect the implementation more
accurately than the current LaTeX.

---

## 7. Examples

### 7.1 Examples in `graph-v3/examples/`

- `dijkstra_clrs_example.cpp` — Dijkstra example using CLRS graph
- `mst_usage_example.cpp` — MST (Kruskal + Prim) usage
- `basic_usage.cpp` — General graph usage

### 7.2 Examples in `D3128_Algorithms/src/`

The LaTeX includes `.hpp` files that serve as prototypes/signatures, not runnable code. No
example `.cpp` files exist in the LaTeX source tree for algorithms.

**Action:** Consider whether to add runnable examples to the proposal or reference the
graph-v3 examples.

---

## 8. Prioritized Task List

### Phase 1: Critical Corrections (Design Shift, Signatures & Concepts)

0. **Replace reference-based types with value-based descriptors** — Remove all uses of `vertex_reference_t<G>`, `edge_reference_t<G>`, `vertex_info`, and `edge_info` from the LaTeX. Replace with `vertex_t<G>`, `edge_t<G>`, `vertex_data`, and `edge_data` per §0. This is the most far-reaching change and should be done first.
1. **Update visitor concepts** — Change from 1-arg `(vertex_info/edge_info wrapper)` to 2-arg `(const G&, const vertex_t<G>&)` / `(const G&, const edge_t<G>&)` form in all visitor `.hpp` files per §1.1.
2. **Update weight function arity** — Change from 1-arg `weight(uv)` to 2-arg `weight(g, uv)` in `basic_edge_weight_function`, `edge_weight_function`, and all Dijkstra/Bellman-Ford signatures per §1.2.
3. **Fix topological sort** — Replace Predecessors with OutputIterator, add bool return, add full-graph variant, remove helpers.
4. **Fix BFS complexity** — O(V+E), not O((E+V) log V).
5. **Fix topological sort complexity** — O(V+E), not O((E+V) log V).
6. **Rename Kosaraju** — `strongly_connected_components` → `kosaraju`; add bidirectional overload.
7. **Fix Kruskal syntax error** — Missing comma in template parameter list.
8. **Fix Label Propagation syntax errors** — Missing commas.

### Phase 2: Return Types & Attributes

9. **Add return types** — `connected_components` → `size_t`, `maximal_independent_set` → `size_t`, `kruskal` → `pair<EV, size_t>`, `prim` → weight type.
10. **Add `[[nodiscard]]`** — To Bellman-Ford, topological sort, triangle count, kruskal, prim.
11. **Add `noexcept`** — To triangle count.
12. **Add `requires` clauses** — `ordered_vertex_edges<G>` for triangle count; `equality_comparable` + `uniform_random_bit_generator` for label propagation; `invocable` for Jaccard.

### Phase 3: Parameters & Naming

13. **Parameter naming** — Rename `source` → `seed` in MIS and Prim for consistency with implementation.
14. **Parameter passing** — Change `vertex_id_t<G>` from by-value to `const vertex_id_t<G>&` in single-source overloads.
15. **Add default values** — `seed = 0` for MIS and Prim.
16. **Add weight function** to Prim's algorithm signature.

### Phase 4: Structural Decisions

17. **Decide on Tarjan SCC** — Keep, remove, or mark as future.
18. **Decide on afforest** — Add to proposal or defer.
19. **Decide on inplace_kruskal** — Add to proposal or defer.
20. **Decide on `_null_range_type`** — Document as exposition-only or omit.
21. **Decide on `_id` visitor variants** — Include in proposal or omit.

### Phase 5: Description & Documentation

22. **Update Effects/Returns/Preconditions** for each changed algorithm.
23. **Update algorithm selection table** (Tier 1 list) based on final decisions.
24. **Update revision history** for D3128r4 (unreleased — all changes are incorporated into this revision).
25. **Cross-reference** with user-guide docs for accuracy of descriptions.
26. **Coordinate cross-paper updates** — The reference→descriptor design shift (§0) also affects D3129 (Views), D3127 (Terminology/GCI), and D3130 (Container Interface). Ensure `vertex_reference_t<G>`, `edge_reference_t<G>`, `vertex_info`, and `edge_info` are replaced consistently across all papers.

---

## 9. Summary of Syntax Errors in Current LaTeX Source Files

| File                                                  | Error                                                                                                                                                  |
| ----------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `D3128_Algorithms/src/lp.hpp`                         | Line 16: Missing comma between `Gen` and `class T` template parameters                                                                                 |
| `D3128_Algorithms/src/lp.hpp`                         | Line 19: Missing comma after `empty_label` parameter                                                                                                   |
| `D3128_Algorithms/src/mst.hpp`                        | Line 7: `template <index_edgelist_range IELR, index_edgelist_range OELR T, CompareOp>` — missing comma/separation between `OELR`, `T`, and `CompareOp` |
| `D3128_Algorithms/src/breadth_first_search_multi.hpp` | `requires` clause appears on same line as return type `void`                                                                                           |
| `D3128_Algorithms/src/topological_sort.hpp`           | `convertible_to<...>` constraint on wrong line — appears before `void` return type                                                                     |
