# GCI Update — Implementation Plan

## Governance

| Item | Value |
|------|-------|
| Strategy | `agents/gci_update_strategy.md` |
| Branch | `gci_paper` (P1709 repo) |
| Authoritative source | graph-v3 implementation |
| Scope | D3130 paper + source files, cross-paper fixups, graph-v3 implementation changes |
| Verification | LaTeX compiles (`make D3130_Container_Interface`), graph-v3 builds and tests pass |

### Commit discipline

Each phase ends with a commit. Phases are ordered so every intermediate commit leaves
the LaTeX compilable (possibly with `\phil` TODO notes for forward references, but no
broken `\lstinputlisting` or undefined `\tcode` references). Implementation changes
have their own phase with a separate commit in the graph-v3 repo.

### Verification gates

| Gate | Command | When |
|------|---------|------|
| LaTeX builds | `make D3130_Container_Interface` in P1709 | After every paper phase |
| graph-v3 builds | `cmake --build build/linux-gcc-debug` in graph-v3 | After Phase I |
| graph-v3 tests | `ctest --test-dir build/linux-gcc-debug` in graph-v3 | After Phase I |

Implementation gate precondition (graph-v3): ensure `build/linux-gcc-debug` is already
configured, or run `cmake -S . -B build/linux-gcc-debug -DCMAKE_BUILD_TYPE=Debug` first.

---

## Phase overview

| Phase | Name | Repo | Depends on | Risk | Status |
|-------|------|------|------------|------|--------|
| A | Source file rewrite | P1709 | — | Low — self-contained `.hpp` files | ✅ Done (`c5fc672`) |
| B | Type alias & traits tables | P1709 | A | Low — table-only edits | ✅ Done (`81014ef`) |
| C | Function tables & CPOs | P1709 | B | Medium — cross-references with concepts | ✅ Done (`58ce4ab`) |
| D | Concept sections | P1709 | A | Medium — concepts reference source files | ✅ Done (`8de6a14`) |
| E | Descriptor section rewrite | P1709 | A | Medium — large prose rewrite | ⬜ Not started |
| F | Edgelist section | P1709 | A, B | Medium — namespace change + pattern rename | ⬜ Not started |
| G | New sections | P1709 | B, D | Low — additive | ⬜ Not started |
| H | Prose audit & cleanup | P1709 | A–G | Low — mechanical search-and-replace | ⬜ Not started |
| I | Implementation changes | graph-v3 | — (independent) | Medium — must pass tests | ⬜ Not started |
| J | Cross-paper fixups | P1709 | H | Low — mechanical | ⬜ Not started |

Phases A–H are sequential within P1709 (each committed separately).
Phase I is independent and can be done before, after, or interleaved with the paper phases.
Phase J is last because it touches other papers and should only run once D3130 is stable.

---

## Phase A — Source File Rewrite

**Goal:** Replace all 11 `.hpp` files under `D3130_Container_Interface/src/` so that
`\lstinputlisting` directives in the LaTeX produce correct output immediately.

### Files to delete

| File | Reason |
|------|--------|
| `concepts_edges_before.hpp` | All `basic_*` edge concepts removed |
| `concepts_basic_adj_list.hpp` | All `basic_*` / `sourced_*` adj list concepts removed |

### Files to rewrite in place

| File | Old content | New content (modelled on graph-v3) |
|------|-------------|-------------------------------------|
| `concepts_edges.hpp` | 6 edge concepts (`targeted_edge`, `sourced_edge`, etc.) | Single `edge<G, E>` concept — `requires(G& g, const E& e) { source_id(g, e); target_id(g, e); }` — no trait gate (Q3) |
| `concepts_target_edge_range.hpp` | `targeted_edge_range` + `basic_targeted_edge_range` | `out_edge_range<R, G>` + `in_edge_range<R, G>` — `forward_range<R> && edge<G, range_value_t<R>>` |
| `concepts_vertex_range.hpp` | `_common_vertex_range`, `vertex_range`, `index_vertex_range` | `vertex<G, V>`, `vertex_range`, `index_vertex_range`, `mapped_vertex_range` |
| `concepts_adj_list.hpp` | `adjacency_list`, `index_adjacency_list`, `sourced_*` | `adjacency_list`, `index_adjacency_list`, `bidirectional_adjacency_list`, `index_bidirectional_adjacency_list`, `mapped_adjacency_list`, `mapped_bidirectional_adjacency_list` |
| `descriptor.hpp` | Single `descriptor<InnerIter>` (129 lines) | `vertex_descriptor<VertexIter>` + `edge_descriptor<EdgeIter, VertexIter, EdgeDirection>` with exposition-only internals (Q1) |
| `descriptor_view.hpp` | `descriptor_view` + `descriptor_subrange_view` (11 lines) | `vertex_descriptor_view` + `edge_descriptor_view` |
| `edgelist_concepts.hpp` | 1-arg `sourced_edgelist`, `sourced_index_edgelist`, `has_edge_value` | 2-arg `basic_sourced_edgelist`, `basic_sourced_index_edgelist`, `has_edge_value` |
| `edgelist_types.hpp` | 1-arg type aliases, includes `edge_reference_t<EL>` | 2-arg type aliases, remove `edge_reference_t<EL>`, add `raw_vertex_id_t<EL>` |
| `edgelist_typetraits.hpp` | `is_directed` only | Same (no change needed) |

### New files to add

| File | Content |
|------|---------|
| `descriptor_traits.hpp` | `is_vertex_descriptor`, `is_edge_descriptor`, `is_descriptor`, `vertex_descriptor_type`, `edge_descriptor_type`, `random_access_descriptor`, `iterator_based_descriptor` — modelled on `graph-v3/include/graph/adj_list/descriptor_traits.hpp` |

### Procedure

1. Delete `concepts_edges_before.hpp` and `concepts_basic_adj_list.hpp`.
2. Rewrite the 9 files listed above, modelling each on the corresponding graph-v3 header
   but simplified to exposition-only level (Q1). Use `// For exposition only` comment
   convention matching the existing style.
3. Create `descriptor_traits.hpp`.
4. Update any `\lstinputlisting` directives in `container_interface.tex` that reference
   deleted files — either remove the directive or redirect it.
5. **Verify:** `make D3130_Container_Interface` builds.
6. **Commit:** `"Phase A: Rewrite D3130 source files to match graph-v3"`

### Safety notes

- The LaTeX `\lstinputlisting` directives reference files by exact name. Deleting a file
  without removing its `\lstinputlisting` will break the build. Step 4 handles this.
- The new source files are the foundation for all subsequent phases. Getting them right
  means the concept / type alias sections can reference them directly.

---

## Phase B — Type Alias & Traits Tables

**Goal:** Update Tables 2 (type aliases) and the traits table to match the implementation.

### Type alias table changes (Table `graph_type`)

| Action | Row(s) |
|--------|--------|
| Remove | `graph_reference_t<G>`, `vertex_reference_t<G>`, `edge_reference_t<G>` |
| Update | `vertex_edge_range_t<G>` → note as alias for `out_edge_range_t<G>` |
| Update | `vertex_edge_iterator_t<G>` → note as alias for `out_edge_iterator_t<G>` |
| Update | `edge_t<G>` → note as alias for `out_edge_t<G>`, now a descriptor type |
| Update | `vertex_id_t<G>` → `remove_cvref_t<decltype(vertex_id(g, vertex_t<G>))>` |
| Add | `out_edge_range_t<G>` = `decltype(out_edges(g, vertex_t<G>))` |
| Add | `out_edge_iterator_t<G>` = `iterator_t<out_edge_range_t<G>>` |
| Add | `out_edge_t<G>` = `range_value_t<out_edge_range_t<G>>` |
| Add | `in_edge_range_t<G>`, `in_edge_iterator_t<G>`, `in_edge_t<G>` (analogous) |
| Add | `vertex_id_store_t<G>` — normative (Q5) |
| Add | *`raw-vertex-id-type`* — exposition-only italic (Q5) |

### Traits table changes (Table `graph_traits`)

| Action | Row(s) |
|--------|--------|
| Remove | `define_unordered_edge<G>`, `unordered_edge<G>`, `ordered_edge<G>` |
| Keep | `define_adjacency_matrix<G>`, `is_adjacency_matrix<G>`/`_v`, `adjacency_matrix<G>` (Q4) — add note that reference implementation has not yet added support |
| Update | `has_contains_edge<G>` → `has_contains_edge<G, V>` (second template parameter) |
| Add | `has_in_degree<G>` / `_v` |
| Add | `has_find_in_edge<G>` / `_v` |
| Add | `has_contains_in_edge<G, V>` / `_v` |
| Add | `has_basic_queries<G>` / `_v` — `degree + find_vertex + find_vertex_edge` |
| Add | `has_full_queries<G>` / `_v` — `has_basic_queries + contains_edge` |

### Procedure

1. Edit the type alias `\begin{tabular}` block in `container_interface.tex` (~line 143–170).
2. Edit the traits `\begin{tabular}` block (~line 99–120).
3. **Verify:** `make D3130_Container_Interface` builds.
4. **Commit:** `"Phase B: Update type alias and traits tables"`

---

## Phase C — Function Tables & CPOs

**Goal:** Update the three function tables (Graph, Vertex, Edge) and the edgelist function
table to match the implementation's CPO names and signatures.

### Graph function table changes (Table `graph_func`)

| Action | Row |
|--------|-----|
| Rename | `has_edge(g)` → `has_edges(g)` |
| Update | `num_edges(g)` default uses `out_edges(g,u)` |

### Vertex function table changes (Table `vertex_func`)

| Action | Row |
|--------|-----|
| Rename | `edges(g,u)` → `out_edges(g,u)` (note `edges` as backward-compatible alias) |
| Rename | `edges(g,uid)` → `out_edges(g,uid)` |
| Rename | `degree(g,u)` → `out_degree(g,u)` (note `degree` as alias) |
| Rename | `degree(g,uid)` → `out_degree(g,uid)` |
| Add | `in_edges(g,u)` / `in_edges(g,uid)` |
| Add | `in_degree(g,u)` / `in_degree(g,uid)` |
| Update | All `vertex_edge_range_t<G>` references → `out_edge_range_t<G>` |

### Edge function table changes (Table `edge_func`)

| Action | Row |
|--------|-----|
| Rename | `find_vertex_edge(g,u,vid)` → `find_out_edge(g,u,vid)` |
| Rename | `find_vertex_edge(g,uid,vid)` → `find_out_edge(g,uid,vid)` |
| Rename | `contains_edge(g,uid,vid)` → `contains_out_edge(g,uid,vid)` |
| Update | `source_id(g,uv)` — remove "n/a, optional", make mandatory for descriptor-based edges (Q8) |
| Add | `source(g,uv)` CPO |
| Add | `find_in_edge(g,u,vid)`, `find_in_edge(g,uid,vid)`, `find_in_edge(g,u,v)` |
| Add | `contains_in_edge(g,u,vid)`, `contains_in_edge(g,uid,vid)` |
| Update | Remove the `\hdashline` / "only available when optional `source_id`" note |
| Update | All `edge_reference_t<G>` in default implementations → `edge_t<G>` |
| Update | Pass descriptors by value in signatures (Q2) |

### Edgelist function table changes (Table `edgelist_func`)

| Action | Row |
|--------|-----|
| Update | `target_id(e)` → `target_id(el, uv)` (2-arg) |
| Update | `source_id(e)` → `source_id(el, uv)` (2-arg) |
| Update | `edge_value(e)` → `edge_value(el, uv)` (2-arg) |
| Update | `contains_edge` lambda: `edge_reference_t<EL>` → raw element type |

### Procedure

1. Edit Graph function table (~line 195–207).
2. Edit Vertex function table (~line 217–235).
3. Edit Edge function table (~line 248–280).
4. Edit Edgelist function table (~line 489–505).
5. Update prose between/after tables that references old CPO names.
6. **Verify:** `make D3130_Container_Interface` builds.
7. **Commit:** `"Phase C: Update function tables and CPO names"`

---

## Phase D — Concept Sections

**Goal:** Rewrite the concept subsections (§1.1–§1.3 and §2.2) to reference the new source
files and explain the simplified concept hierarchy.

### Adjacency list concept sections

| Section | Change |
|---------|--------|
| §1.1.1 "Edge Concepts" | Reference new `concepts_edges.hpp` (single `edge<G,E>`). Update the prose explaining why return types are not validated — still applies. Remove the `concepts_edges_before.hpp` reference. |
| §1.1.1 "Edge Range Concepts" | Reference new `concepts_target_edge_range.hpp` (now `out_edge_range` + `in_edge_range`). Update prose. |
| §1.1.2 "Vertex Concepts" | Reference new `concepts_vertex_range.hpp` (now with `vertex<G,V>`, `mapped_vertex_range`). |
| §1.1.3 "Adjacency List Concepts" | Reference new `concepts_adj_list.hpp`. Remove the `\phil` verification note. Remove the `\lstinputlisting` for `concepts_basic_adj_list.hpp`. Explain the removal of `basic_*`/`sourced_*` variants — descriptor design makes them unnecessary. |
| §2.2 "Edgelist Concepts" | Reference new `edgelist_concepts.hpp`. Explain 2-arg form. |

### Procedure

1. Update `\lstinputlisting` directives (some files renamed/deleted in Phase A).
2. Rewrite surrounding prose for each concept subsection.
3. **Verify:** `make D3130_Container_Interface` builds.
4. **Commit:** `"Phase D: Rewrite concept sections"`

---

## Phase E — Descriptor Section Rewrite

**Goal:** Replace the "Vertex and Edge Descriptor Views" subsection (~line 362–410) with a
comprehensive descriptor exposition that includes the "Why Descriptors?" motivation (§0.2a
of the strategy), the comparison table, the split descriptor architecture, pass-by-value
convention (Q2), and the exposition-only styling (Q1).

### Content outline for the new section

1. **Motivation** — Why descriptors? Comparison table (iterators vs descriptors). Implicit
   vertex support. Automatic pattern recognition.
2. **Descriptor types** — `vertex_descriptor<VertexIter>` and
   `edge_descriptor<EdgeIter, VertexIter, EdgeDirection>` with exposition-only data members.
   Observable interface: construction, `vertex_id()`, comparison, increment.
3. **Descriptor traits** — `\lstinputlisting{descriptor_traits.hpp}`. Brief explanation.
4. **Descriptor views** — `vertex_descriptor_view` and `edge_descriptor_view`.
   `\lstinputlisting{descriptor_view.hpp}`.
5. **Pass-by-value convention** — Descriptors are ≤16 bytes, trivially copyable, passed by
   value in CPO signatures. `const&` only in `requires` clauses.

### Procedure

1. Replace the existing "Vertex and Edge Descriptor Views" subsection prose.
2. Update `\lstinputlisting{descriptor.hpp}` to show the new split descriptors.
3. Add `\lstinputlisting{descriptor_traits.hpp}`.
4. Update `\lstinputlisting{descriptor_view.hpp}`.
5. **Verify:** `make D3130_Container_Interface` builds.
6. **Commit:** `"Phase E: Rewrite descriptor section with motivation and split architecture"`

---

## Phase F — Edgelist Section

**Goal:** Update the entire Edgelist Interface section (§2) to match the implementation.

### Changes

| Subsection | Change |
|------------|--------|
| §2 intro | Update `source_id(e)` → `source_id(el, uv)` prose |
| §2.1 Namespace | Change `std::graph::edgelist` → `std::graph::edge_list` (Q6). Remove the `\phil` merge note on line 417. |
| §2.2 Concepts | Already handled by Phase D (just verify) |
| §2.3 Traits | No change (edgelist traits unchanged) |
| §2.4 Types | Update type alias table (Table `edgelist_type`): remove `edge_reference_t<EL>`, update `edge_value_t` and `vertex_id_t` to 2-arg form, add `raw_vertex_id_t<EL>` |
| §2.5 Functions | Already handled by Phase C (just verify) |
| §2.6 Pattern section | Rename `edge_info` → `edge_data` (4 occurrences). Rename `vertex_info` → `vertex_data`. Add `pair<T,T>` pattern. Add `edge_data` reference variants. |

### Procedure

1. Update namespace prose and remove `\phil` note.
2. Update edgelist type alias table.
3. Rename `edge_info` → `edge_data` in pattern section prose.
4. **Verify:** `make D3130_Container_Interface` builds.
5. **Commit:** `"Phase F: Update edgelist section — namespace, 2-arg CPOs, edge_data patterns"`

---

## Phase G — New Sections

**Goal:** Add content for new items that have no existing LaTeX counterpart.

### New subsections to add

| Subsection | Location | Content |
|------------|----------|---------|
| §1.4 "Classes and Structs" expansion | After existing `graph_error` | Add `vertex_data`, `edge_data`, `neighbor_data` struct families with specialization tables. Add `copyable_vertex_t`, `copyable_edge_t`, `copyable_neighbor_t` aliases. |
| §1.5 "Value Function Concepts" | New subsection after Classes | `vertex_value_function<VVF, G, V>` and `edge_value_function<EVF, G, E>` — from `graph_concepts.hpp` |
| §1.6 "Vertex Property Map" | New subsection | `vertex_property_map<G, T>` type alias, `make_vertex_property_map`, `vertex_property_map_contains`, `vertex_property_map_get` (Q7) |
| §1.7 "Vertex ID Determination" update | Existing subsection | Rewrite to descriptor-based approach (pattern detection on descriptors, not on nested range types) |

### Procedure

1. Add each new subsection to `container_interface.tex`.
2. Adjust section numbering / labels as needed.
3. **Verify:** `make D3130_Container_Interface` builds.
4. **Commit:** `"Phase G: Add data structs, value function concepts, vertex property map"`

---

## Phase H — Prose Audit & Cleanup

**Goal:** Systematic search-and-replace pass through all D3130 text, plus cleanup of stale
commentary, and revision history update.

### Search-and-replace pass

| Old term | New term | Expected locations |
|----------|----------|--------------------|
| `vertex_reference_t` | `vertex_t` | Prose, concept definitions, function sigs |
| `edge_reference_t` | `edge_t` | Same |
| `vertex_info` | `vertex_data` | Verify none remain in D3130 (expected 0) |
| `edge_info` | `edge_data` | Edgelist pattern section (4 occurrences in D3130) |
| `targeted_edge` | `edge<G,E>` | Concept references in prose |
| `sourced_edge` | (remove or replace) | Concept references in prose |
| `basic_*` concepts | (remove references) | Prose |
| `std::graph::edgelist` | `std::graph::edge_list` | Namespace references |
| `edges(g,u)` (as primary name) | `out_edges(g,u)` | Prose (function tables handled in Phase C) |
| `degree(g,u)` (as primary name) | `out_degree(g,u)` | Prose |
| `find_vertex_edge` | `find_out_edge` | Prose |
| `contains_edge` (adj list) | `contains_out_edge` | Prose |
| `has_edge(g)` | `has_edges(g)` | Prose |
| `inner_value()` | `underlying_value(container)` / `inner_value(container)` | Descriptor prose |

### Additional cleanup

1. Update adjacency matrix prose — revise `contains_out_edge` discussion to note O(1)
   guarantee when `adjacency_matrix<G>` is satisfied (Q4). Note that the reference
   implementation has not yet added adjacency matrix support.
2. Review "Using Existing Data Structures" section for accuracy.
3. Remove any remaining `\phil` notes.
4. Update `D3130_Container_Interface/tex/revision.tex` with a revision entry summarizing
   the changes.

### Procedure

1. Run `grep -n` for each old term to find all occurrences.
2. Edit each occurrence.
3. Review adjacency matrix prose.
4. Review "Using Existing Data Structures" section.
5. Remove stale `\phil` notes.
6. Update revision history.
7. **Verify:** `make D3130_Container_Interface` builds.
8. **Commit:** `"Phase H: Prose audit — rename all obsolete types, update revision history"`

---

## Phase I — Implementation Changes (graph-v3)

**Goal:** Update the graph-v3 implementation to match Q3 (drop trait gate on `edge<G,E>`)
and ensure consistency with the paper.

### I.1 — Drop `is_edge_descriptor_v` gate on `edge<G,E>`

**File:** `include/graph/adj_list/adjacency_list_concepts.hpp`, line 46

**Current:**
```cpp
template <class G, class E>
concept edge = is_edge_descriptor_v<std::remove_cvref_t<E>> && requires(G& g, const E& e) {
  source_id(g, e);
  source(g, e);
  target_id(g, e);
  target(g, e);
};
```

**New:**
```cpp
template <class G, class E>
concept edge = requires(G& g, const E& e) {
  source_id(g, e);
  target_id(g, e);
};
```

**Impact assessment:**
- Removes the trait gate — any type satisfying `source_id`/`target_id` CPOs qualifies.
- `source(g,e)` and `target(g,e)` are dropped from the concept requirement. Algorithms that
  need vertex descriptors (not just IDs) should add those requirements directly.
- Tests in `test_descriptor_traits.cpp` test the *trait* (`is_edge_descriptor_v`), not the
  *concept* — they remain valid.
- Tests in `test_target_id_cpo.cpp` use `requires(is_edge_descriptor_v<E>)` as a guard —
  these remain valid since the trait still exists; it's just no longer in the concept.
- Algorithms using `edge<G,E>` in `requires` clauses will accept a broader set of types.
  Need to run full test suite to verify no regressions.

### I.2 — Verify edge_list interop

After dropping the trait gate, edge_list raw elements (tuples, pairs, `edge_data`) should
be testable against `edge<G,E>`. Add a static assertion test:

```cpp
// In a new or existing edge_list test:
using EL = std::vector<std::tuple<int, int, double>>;
static_assert(edge<EL, std::tuple<int, int, double>>);
```

### I.3 — Algorithms that need `source(g,e)` / `target(g,e)`

Search all algorithm headers for uses of `source(g,` and `target(g,` to identify any that
need the vertex-descriptor operations. These algorithms should add explicit requirements
beyond `edge<G,E>`:

```cpp
template <class G, class E>
  requires edge<G, E> && requires(G& g, const E& e) { source(g, e); target(g, e); }
void algorithm_needing_vertex_descriptors(G& g, E e);
```

### Procedure

1. Enter graph-v3 workspace root (`/home/phil/dev_graph/graph-v3`).
2. Ensure build directory is configured:
  `cmake -S . -B build/linux-gcc-debug -DCMAKE_BUILD_TYPE=Debug` (if needed).
3. Edit `adjacency_list_concepts.hpp` line 46.
4. Build: `cmake --build build/linux-gcc-debug`.
5. Run tests: `ctest --test-dir build/linux-gcc-debug`.
6. If tests fail, analyse each failure:
   - If an algorithm relied on `source(g,e)` / `target(g,e)` being in the concept, add
     explicit requirements to that algorithm.
   - If a test assumed `edge<G,E>` requires `is_edge_descriptor_v`, update the test.
7. Add edge_list interop static assertion test.
8. Run tests again to confirm green.
9. **Commit (graph-v3):** `"Relax edge<G,E> concept — drop is_edge_descriptor_v gate (Q3)"`

### Safety notes

- This is the **only** implementation change required by the Q1-Q8 decisions. All other Qs
  affect only the paper text.
- The trait `is_edge_descriptor_v` itself is NOT removed — it remains available for code
  that specifically needs to detect adj_list descriptors. Only its role as a *concept gate*
  is eliminated.
- If test failures are extensive, this phase can be deferred and the paper updated
  independently. The paper should specify the relaxed concept regardless of whether the
  implementation has caught up.

---

## Phase J — Cross-Paper Fixups (Allowed Scope)

**Goal:** Update references to changed D3130 types in the algorithms paper and shared
specification text, while keeping this plan within the approved scope.

### Affected files

| File | Occurrences | Changes |
|------|-------------|---------|
| `tex/conventions.tex` | 4 | `vertex_reference_t<G>` → `vertex_t<G>`, `edge_reference_t<G>` → `edge_t<G>`, `vertex_info` → `vertex_data`, `edge_info` → `edge_data` in projection function descriptions |
| `tex/specification.tex` | 6 | `vertex_reference_t<G>` → `vertex_t<G>` in function signatures and concept definitions |
| `D3128_Algorithms/tex/algorithms.tex` | 2 | `edge_reference_t<G>` → `edge_t<G>` in weight function concept |
| `D3128_Algorithms/tex/revision.tex` | 2 | Update revision note referencing the type changes |

### Procedure

1. Fix `tex/conventions.tex` (4 occurrences).
2. Fix `tex/specification.tex` (6 occurrences).
3. Fix `D3128_Algorithms/tex/algorithms.tex` (2 occurrences).
4. Fix `D3128_Algorithms/tex/revision.tex` (2 occurrences).
5. **Verify:** `make D3128_Algorithms` and `make` for shared top-level docs affected.
6. **Commit:** `"Phase J: Cross-paper fixups for algorithms and shared spec text"`

### Optional extension (separate plan)

If broader editorial synchronization is desired, create a dedicated Views-paper plan for
`D3129_Views/tex/views.tex` and `D3129_Views/tex/revision.tex` (large-volume rename and
potential source-listing impacts).

### Safety notes

- For bulk replacements, avoid relying on `sed` word-boundary `\b` behavior (tool-dependent).
  Prefer `perl -pe` with explicit token boundaries, then spot-check all replacements.
- Keep this phase scoped to Algorithms + shared specification files. Treat Views-paper
  renames and any `\lstinputlisting` source-file rename fallout as a separate effort.

---

## Phase dependency diagram

```
Phase A (source files)
  ├── Phase B (type alias & traits tables)
  │     └── Phase C (function tables & CPOs)
  │           └── Phase F (edgelist section) ← also needs A
  ├── Phase D (concept sections)
  │     └── Phase G (new sections) ← also needs B
  └── Phase E (descriptor section)

All A–G ──→ Phase H (prose audit & cleanup)
                └── Phase J (cross-paper fixups)

Phase I (implementation) ──→ independent
```

---

## Risk register

| Risk | Likelihood | Impact | Mitigation |
|------|------------|--------|------------|
| `\lstinputlisting` broken by file rename/delete | High (if not careful) | Build failure | Phase A explicitly handles all directives. Verify build after. |
| Bulk rename introduces false positives | Medium | Incorrect prose | Prefer `perl -pe` with explicit token boundaries; then spot-check every replacement. |
| Dropping trait gate breaks graph-v3 tests | Medium | Test failures | Run full test suite in Phase I. If failures are extensive, defer. |
| Section numbering drift after adding new sections | Low | LaTeX warnings | Use `\label` / `\ref` for all cross-references. |
| Adjacency matrix traits have no implementation backing | Low | Reviewer confusion | Add explicit note in traits table: "Reference implementation pending." |
| Cross-paper edits create merge conflicts with other branches | Medium | Git conflicts | Phase J is last and on its own commit, easy to rebase. |
