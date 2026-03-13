# D3129 Views — Implementation Plan

**Source of truth:** `agents/views_update_strategy.md`
**Target document:** `D3129_Views/tex/views.tex` and all `D3129_Views/src/*.hpp`
**Reference implementation:** `graph-v3/include/graph/views/` and `graph-v3/include/graph/graph_data.hpp`

All changes are applied to D3129r3 before publication. The implementation is the definitive
source; the LaTeX must conform to it.

---

## Governance

| Item | Value |
|------|-------|
| Strategy | `agents/views_update_strategy.md` |
| Branch | `view_paper` (P1709 repo) |
| Authoritative source | graph-v3 implementation |
| Scope | D3129 paper + source files |
| Verification | LaTeX compiles (`latexmk -output-directory=pdf -aux-directory=aux D3129_Views.tex`), no broken `\lstinputlisting` |

### Commit discipline

Each phase ends with a commit. Phases are ordered so every intermediate commit leaves
the LaTeX compilable (possibly with `\phil` TODO notes for forward references, but no
broken `\lstinputlisting` or undefined `\tcode` references).

### Verification gates

| Gate | Command | When |
|------|---------|------|
| LaTeX builds | `latexmk -output-directory=pdf -aux-directory=aux D3129_Views.tex` in P1709 | After every phase |

### Naming convention

All structured bindings and prose must follow this convention consistently:

| Symbol | Meaning |
|--------|---------|
| `u` | Source vertex (descriptor) |
| `v` | Target vertex (descriptor) |
| `uid` | Source vertex id |
| `vid` | Target vertex id |
| `uv` | Edge (from u to v) |

**Context rules:**
- **Vertex-only iteration** (vertexlist, vertex search views): use `uid`/`u` for the iterated vertex.
- **Incidence / edge iteration**: `vid` = target id, `uv` = edge.
- **Neighbor iteration**: `vid` = target id, `v` = target vertex.
- **Edgelist iteration**: `uid` = source id, `vid` = target id, `uv` = edge.
- **Basic views** (id-only): same convention but without descriptor bindings (e.g., `[uid]`, `[vid]`, `[uid, vid]`).

---

## Phase overview

| Phase | Name | Depends on | Risk | Status |
|-------|------|------------|------|--------|
| A | Source file rewrite (data structs) | — | Low — 3 small `.hpp` files | Done (ac5abdd) |
| B | Data structs section rewrite | A | Medium — largest prose rewrite, tables | Done (1152806) |
| C | Graph views update (vertexlist, incidence, neighbors, edgelist) | B | Medium — table + prose changes | Done (6f50537) |
| D | Search common types update | B | Low — enum + member functions | Done (ce8f76a) |
| E | DFS / BFS views update | C, D | Medium — removals + binding changes | Done (4ee7cc1) |
| F | Topological sort views update | C, D | Medium — signature changes + new `_safe` factories | Done (8552f76) |
| G | New content (basic variants, in_ variants, transpose, pipe syntax) | C, D, E, F | Medium — additive | Done (`fb01712`) |
| H | Introduction, prose audit & cleanup | A–G | Low — mechanical | Done (`51b85b0`) |
| I | Revision history & config | H | Low — additive | Done (`81cb596`) |
| J | Cross-paper fixups (conventions.tex, D3126 C++23 note) | I | Low — mechanical | Done (`a3430a4`) |

Phases A–I are sequential within P1709 (each committed separately).
Phase J is last because it touches shared files and should only run once D3129 is stable.

---

## Phase A — Source File Rewrite

**Goal:** Replace the 3 `.hpp` files under `D3129_Views/src/` so that `\lstinputlisting`
directives produce correct output immediately.

### Files to replace with renamed counterparts

| Old File | New File | Old Content | New Content |
|----------|----------|-------------|-------------|
| `vertex_info.hpp` | `vertex_data.hpp` | `vertex_info<VorVId, VV>` (2 params, 8 lines) | `vertex_data<VId, V, VV>` (3 params) primary template |
| `edge_info.hpp` | `edge_data.hpp` | `edge_info<VorVId, Sourced, E, EV>` (4 params, 12 lines) | `edge_data<VId, Sourced, E, EV>` (4 params) primary template with renamed members |
| `neighbor_info.hpp` | `neighbor_data.hpp` | `neighbor_info<VorVId, Sourced, VV>` (3 params, 10 lines) | `neighbor_data<VId, Sourced, V, VV>` (4 params) primary template |

### Content for new source files

Model each on the graph-v3 `graph_data.hpp` implementation but simplified to
exposition-only level. Per strategy Q1 recommendation (b), show the primary template
plus ~4 key specializations per type (the ones views actually produce).

**`vertex_data.hpp`** — primary template + key specializations:
```cpp
template <class VId, class V, class VV>
struct vertex_data {
  using id_type     = VId;    // e.g. vertex_id_t<G> or void
  using vertex_type = V;      // e.g. vertex_t<G> or void
  using value_type  = VV;     // e.g. invoke_result of vvf, or void

  id_type     id;
  vertex_type vertex;
  value_type  value;
};
```
Key specializations to show: `<VId, V, void>`, `<VId, void, void>`, `<VId, void, VV>`.

**`edge_data.hpp`** — primary template:
```cpp
template <class VId, bool Sourced, class E, class EV>
struct edge_data {
  using source_id_type = VId;  // e.g. vertex_id_t<G> when Sourced==true, or void
  using target_id_type = VId;  // e.g. vertex_id_t<G>
  using edge_type      = E;    // e.g. edge_t<G> or void
  using value_type     = EV;   // e.g. invoke_result of evf, or void

  source_id_type source_id;
  target_id_type target_id;
  edge_type      edge;
  value_type     value;
};
```
Key specializations: `<VId, false, E, void>` (incidence), `<VId, true, E, void>` (edgelist),
`<void, false, E, void>` (search edge views), `<VId, false, void, void>` (basic incidence).

**`neighbor_data.hpp`** — primary template:
```cpp
template <class VId, bool Sourced, class V, class VV>
struct neighbor_data {
  using source_id_type = VId;  // e.g. vertex_id_t<G> when Sourced==true, or void
  using target_id_type = VId;  // e.g. vertex_id_t<G>
  using target_type    = V;    // e.g. vertex_t<G> or void
  using value_type     = VV;   // e.g. invoke_result of vvf, or void

  source_id_type source_id;
  target_id_type target_id;
  target_type    target;
  value_type     value;
};
```
Key specializations: `<VId, false, V, void>` (neighbors), `<VId, false, void, void>` (basic).

### Procedure

1. Delete `vertex_info.hpp`, `edge_info.hpp`, `neighbor_info.hpp`.
2. Create `vertex_data.hpp`, `edge_data.hpp`, `neighbor_data.hpp` with primary templates
   and representative specializations.
3. Update `\lstinputlisting` directives in `views.tex` to reference the new filenames:
   - `D3129_Views/src/vertex_info.hpp` → `D3129_Views/src/vertex_data.hpp`
   - `D3129_Views/src/edge_info.hpp` → `D3129_Views/src/edge_data.hpp`
   - `D3129_Views/src/neighbor_info.hpp` → `D3129_Views/src/neighbor_data.hpp`
4. **Verify:** `latexmk -output-directory=pdf -aux-directory=aux D3129_Views.tex` builds.
5. **Commit:** `"Phase A: Rewrite D3129 source files — info structs → data structs"`

### Safety notes

- The LaTeX `\lstinputlisting` directives reference files by exact name. Deleting a file
  without updating its `\lstinputlisting` will break the build. Step 3 handles this.
- Do NOT update surrounding prose or tables yet. Only source files and their
  `\lstinputlisting` references change in this phase.

---

## Phase B — Info Structs Section Rewrite

**Goal:** Rewrite the "Info Structs (Return Types)" section (§2, views.tex lines ~31–240)
to describe the new data struct types, specialization tables, and introductory code examples.

### Section heading change

- `\section{Info Structs (Return Types)}` → `\section{Data Structs (Return Types)}`

### Subsection heading changes

| Old | New |
|-----|-----|
| `\subsection{\tcode{struct vertex_info<VId, VV>}}` | `\subsection{\tcode{struct vertex_data<VId, V, VV>}}` |
| `\subsection{\tcode{struct edge_info<VId, Sourced, E, EV>}}` | `\subsection{\tcode{struct edge_data<VId, Sourced, E, EV>}}` |
| `\subsection{\tcode{struct neighbor_info<VId, Sourced, V, VV>}}` | `\subsection{\tcode{struct neighbor_data<VId, Sourced, V, VV>}}` |

### Introductory code examples (lines ~39–78)

Update all code examples to use new types, 2-arg value functions, and correct bindings:

| Old Code | New Code |
|----------|----------|
| `vertex_reference_t<G> u = uu.vertex;` | `vertex_t<G> u = uu.vertex;` |
| `auto vvf = [&g](vertex_reference_t<G> u) { return vertex_value(g,u); };` | `auto vvf = [](const auto& g, vertex_t<G> u) { return vertex_value(g,u); };` |
| `// type is vertex_info<vertex_t<G>, void>` | `// type is vertex_data<vertex_id_t<G>, vertex_t<G>, void>` |
| `for(auto&& uu : vertexlist(g))` | `for(auto&& [uid, u] : vertexlist(g))` |
| `for(auto&& [u] : vertexlist(g))` | `for(auto&& [uid, u] : vertexlist(g))` |
| `for(auto&& [u, value] : vertexlist(g, vvf))` | `for(auto&& [uid, u, val] : vertexlist(g, vvf))` |

### Specialization tables

Replace the existing 2-row `vertex_info` table with an expanded `vertex_data` table
showing the key specializations used by views:

| Template Arguments | Members |
|--------------------|---------|
| `vertex_data<VId, V, VV>` | `id`, `vertex`, `value` |
| `vertex_data<VId, V, void>` | `id`, `vertex` |
| `vertex_data<VId, void, VV>` | `id`, `value` |
| `vertex_data<VId, void, void>` | `id` |

Similarly expand `edge_data` and `neighbor_data` tables.

Update member names in the `edge_data` table:
- `source` → `source_id`
- `target` → `target_id`

### Copyable info structs block (commented-out, lines ~200–240)

Leave the `\begin{comment}...\end{comment}` block in place for now but update internal
references from `vertex_info` → `vertex_data`, `edge_info` → `edge_data`,
`neighbor_info` → `neighbor_data`. This block can be un-commented in a later revision
if needed.

### Procedure

1. Rename section and subsection headings.
2. Rewrite introductory code examples.
3. Replace specialization tables with expanded versions.
4. Update prose references: `vertex_info` → `vertex_data`, `edge_info` → `edge_data`,
   `neighbor_info` → `neighbor_data` throughout §2.
5. Update commented-out block.
6. **Verify:** `latexmk -output-directory=pdf -aux-directory=aux D3129_Views.tex` builds.
7. **Commit:** `"Phase B: Rewrite data structs section — tables, examples, prose"`

### Safety notes

- This is the largest single-phase prose change. Work subsection by subsection.
- After updating each subsection, verify that `\label` references (`\ref{tab:vertex-view}`,
  etc.) still match. Update `\label` names if headings change.

---

## Phase C — Graph Views Update

**Goal:** Update the four graph view subsections (vertexlist, incidence, neighbors, edgelist)
to use the new data struct types, correct structured bindings, and 2-arg value functions.

### vertexlist (views.tex ~lines 245–270)

| Old | New |
|-----|-----|
| `for(auto&& [u] : vertexlist(g))` → `vertex_info<void,V,void>` | `for(auto&& [uid, u] : vertexlist(g))` → `vertex_data<VId,V,void>` |
| `for(auto&& [u,val] : vertexlist(g,vvf))` → `vertex_info<void,V,VV>` | `for(auto&& [uid, u, val] : vertexlist(g,vvf))` → `vertex_data<VId,V,VV>` |
| Same pattern for `(g,first,last)` and `(g,vr)` overloads | Same pattern |

Update surrounding prose: "returning a `vertex_info`" → "returning a `vertex_data`".

### incidence (views.tex ~lines 272–295)

| Old | New |
|-----|-----|
| `for(auto&& [uv] : incidence(g,u))` → `edge_info<void,false,E,void>` | `for(auto&& [vid, uv] : incidence(g,u))` → `edge_data<VId,false,E,void>` |
| `for(auto&& [uv,val] : incidence(g,u,evf))` → `edge_info<void,false,E,EV>` | `for(auto&& [vid, uv, val] : incidence(g,u,evf))` → `edge_data<VId,false,E,EV>` |

Update prose: `edge_info` → `edge_data`.

### neighbors (views.tex ~lines 297–318)

| Old | New |
|-----|-----|
| `for(auto&& [v] : neighbors(g,uid))` → `neighbor_info<void,false,V,void>` | `for(auto&& [vid, v] : neighbors(g,uid))` → `neighbor_data<VId,false,V,void>` |
| `for(auto&& [v,val] : neighbors(g,uid,vvf))` → `neighbor_info<void,false,V,VV>` | `for(auto&& [vid, v, val] : neighbors(g,uid,vvf))` → `neighbor_data<VId,false,V,VV>` |

Update prose: `neighbor_info` → `neighbor_data`, `vertex_info` → `vertex_data` (neighbors
prose incorrectly says `vertex_info` in the LaTeX).

### edgelist (views.tex ~lines 320–340)

| Old | New |
|-----|-----|
| `for(auto&& [u,v,uv] : edgelist(g))` → `edge_info<V,true,E,void>` | `for(auto&& [uid, vid, uv] : edgelist(g))` → `edge_data<VId,true,E,void>` |
| `for(auto&& [u,v,uv,val] : edgelist(g,evf))` → `edge_info<V,true,E,EV>` | `for(auto&& [uid, vid, uv, val] : edgelist(g,evf))` → `edge_data<VId,true,E,EV>` |

Update prose: `edge_info` → `edge_data`. Update `evf(uv)` references to `evf(g,uv)`.

### Procedure

1. Update each subsection table and prose sequentially.
2. Update all value function references from 1-arg to 2-arg in prose.
3. **Verify:** `latexmk -output-directory=pdf -aux-directory=aux D3129_Views.tex` builds.
4. **Commit:** `"Phase C: Update graph view sections — data structs, bindings, 2-arg VVF/EVF"`

---

## Phase D — Search Common Types Update

**Goal:** Update the "Common Types and Functions for Search" subsection to replace free
functions with member functions and rename `size` → `num_visited`.

### Changes

| Old | New |
|-----|-----|
| `void cancel(S search, cancel_search);` | `search.cancel(cancel_search);` |
| `auto depth(S search) -> integral;` | `search.depth() -> size_t;` |
| `auto size(S search) -> integral;` | `search.num_visited() -> size_t;` |

### Code listing update

Replace the free-function listing with member-function declarations. Add the
`cancel_search` enum (unchanged) and the `search_view` concept:

```cpp
template <class V>
concept search_view = requires(V& v, const V& cv) {
    { v.cancel() } -> std::convertible_to<cancel_search>;
    { cv.depth() } -> std::convertible_to<std::size_t>;
    { cv.num_visited() } -> std::convertible_to<std::size_t>;
};
```

### Code example update

Replace the free-function usage example:

| Old | New |
|-----|-----|
| `if(depth(dfs) > 3)` | `if(dfs.depth() > 3)` |
| `cancel(dfs, cancel_search::cancel_branch)` | `dfs.cancel(cancel_search::cancel_branch)` |
| `if(size(dfs) > 1000)` | `if(dfs.num_visited() > 1000)` |
| `std::cout << "Big depth of " << size(dfs)` | `std::cout << "Many visited: " << dfs.num_visited()` |

### Prose updates

- Update the paragraph that says "`size(dfs)` is typically the same as `depth(dfs)`" to
  use member syntax and `num_visited` name.
- Remove the paragraph about sourced views adhering to `basic_sourced_index_edgelist` —
  sourced search views are removed.
- Remove `\phil{Do these apply to all "search" functions?}` note.

### Procedure

1. Update code listing (enum + member functions + concept).
2. Update code example.
3. Update surrounding prose.
4. **Verify:** `latexmk -output-directory=pdf -aux-directory=aux D3129_Views.tex` builds.
5. **Commit:** `"Phase D: Update search common types — member functions, search_view concept"`

---

## Phase E — DFS / BFS Views Update

**Goal:** Update DFS and BFS view tables and prose to match the implementation.

### DFS changes (views.tex ~lines 405–440)

| Old Row | New Row |
|---------|---------|
| `[v] : vertices_dfs(g,source)` → `vertex_info<void,V,void>` | `[u] : vertices_dfs(g,seed)` → `vertex_data<void,V,void>` |
| `[v,val] : vertices_dfs(g,source,vvf)` → `vertex_info<void,V,VV>` | `[u, val] : vertices_dfs(g,seed,vvf)` → `vertex_data<void,V,VV>` |
| `[v,uv] : edges_dfs(g,source)` → `edge_info<V,false,E,void>` | `[uv] : edges_dfs(g,seed)` → `edge_data<void,false,E,void>` |
| `[v,uv,val] : edges_dfs(g,source,evf)` → `edge_info<V,false,E,EV>` | `[uv, val] : edges_dfs(g,seed,evf)` → `edge_data<void,false,E,EV>` |
| `[u,v,uv] : sourced_edges_dfs(g,source)` | **Remove** |
| `[u,v,uv,val] : sourced_edges_dfs(g,source,evf)` | **Remove** |

**Critical:** Edge DFS views change from `[v,uv]` (2 bindings) to `[uv]` (1 binding) —
VId is void, so no vertex id in the binding.

Remove all `\phil` notes in the DFS subsection.

Update parameter name: `source` → `seed`.

### BFS changes (views.tex ~lines 450–487)

Identical structural changes as DFS:

| Old Row | New Row |
|---------|---------|
| `[v] : vertices_bfs(g,source)` → `vertex_info<void,V,void>` | `[u] : vertices_bfs(g,seed)` → `vertex_data<void,V,void>` |
| `[v,val] : vertices_bfs(g,source,vvf)` → `vertex_info<void,V,VV>` | `[u, val] : vertices_bfs(g,seed,vvf)` → `vertex_data<void,V,VV>` |
| `[v,uv] : edges_bfs(g,source)` → `edge_info<V,false,E,void>` | `[uv] : edges_bfs(g,seed)` → `edge_data<void,false,E,void>` |
| `[v,uv,val] : edges_bfs(g,source,evf)` → `edge_info<V,false,E,EV>` | `[uv, val] : edges_bfs(g,seed,evf)` → `edge_data<void,false,E,EV>` |
| `[u,v,uv] : sourced_edges_bfs(g,source)` | **Remove** |
| `[u,v,uv,val] : sourced_edges_bfs(g,source,evf)` | **Remove** |

Remove all `\phil` notes in the BFS subsection (including `depth_limit` note).

### Design note (strategy Q7)

Add a brief "Design Note" paragraph after the DFS/BFS tables explaining that search views
yield descriptors rather than IDs (VId=void). Users obtain IDs via `vertex_id(g, v)`.
Descriptors provide richer access via CPOs.

### Procedure

1. Update DFS table and remove sourced rows.
2. Update BFS table and remove sourced rows.
3. Remove all `\phil` notes in DFS and BFS sections.
4. Add design note paragraph.
5. Update surrounding prose.
6. **Verify:** `latexmk -output-directory=pdf -aux-directory=aux D3129_Views.tex` builds.
7. **Commit:** `"Phase E: Update DFS/BFS views — remove sourced, descriptor-based return"`

---

## Phase F — Topological Sort Views Update

**Goal:** Update topological sort to remove seed parameter, remove sourced variants,
and add `_safe` factory documentation.

### Table changes

| Old Row | New Row |
|---------|---------|
| `[v] : vertices_topological_sort(g,source)` | `[u] : vertices_topological_sort(g)` — no source param |
| `[v,val] : vertices_topological_sort(g,source,vvf)` | `[u, val] : vertices_topological_sort(g,vvf)` |
| `[v,uv] : edges_topological_sort(g,source)` | `[uv] : edges_topological_sort(g)` — VId=void |
| `[v,uv,val] : edges_topological_sort(g,source,evf)` | `[uv, val] : edges_topological_sort(g,evf)` |
| `[u,v,uv] : sourced_edges_topological_sort(g,source)` | **Remove** |
| `[u,v,uv,val] : sourced_edges_topological_sort(g,source,evf)` | **Remove** |

### New: `_safe` factories

Add rows showing cycle-detecting factories that return `std::expected`:

```
vertices_topological_sort_safe(g)       → std::expected<view, vertex_t<G>>
vertices_topological_sort_safe(g, vvf)  → std::expected<view, vertex_t<G>>
edges_topological_sort_safe(g)          → std::expected<view, vertex_t<G>>
edges_topological_sort_safe(g, evf)     → std::expected<view, vertex_t<G>>
```

Use `std::expected` per strategy Q4 recommendation (a). The paper specifies
`std::expected`; the implementation provides backward compatibility to C++20 via
an external `expected` library (e.g., `tl::expected`), switching to `std::expected`
when C++23 or later is used. Add a brief note in the topological sort section
stating this compatibility approach.

### Prose updates

- Update intro prose: "from a given source vertex" → "over all vertices in the graph"
- Note that topo sort does NOT satisfy `search_view` (no `depth()` member).
- Note that `cancel_branch` behaves as `cancel_all` (flat ordering).
- Remove all `\phil` notes.

### Procedure

1. Update table — remove source parameter and sourced rows.
2. Add `_safe` factory rows.
3. Update prose.
4. **Verify:** `latexmk -output-directory=pdf -aux-directory=aux D3129_Views.tex` builds.
5. **Commit:** `"Phase F: Update topological sort — all-vertex, _safe factories, remove sourced"`

---

## Phase G — New Content

**Goal:** Add sections for `basic_` variants, `in_` variants, `transpose` view,
and pipe syntax. Per strategy Q2 recommendation (a), basic variants go in the same
table/subsection as their standard counterparts (split to follow-on table if needed
for page fit). Per strategy Q3 recommendation (c), pipe syntax appears both in each
view subsection and in a summary section.

### G.1 — Basic view variants

Add rows to each graph view table from Phase C:

**vertexlist:**
| New Row | Return |
|---------|--------|
| `for(auto&& [uid] : basic_vertexlist(g))` | `vertex_data<VId,void,void>` |
| `for(auto&& [uid, val] : basic_vertexlist(g,vvf))` | `vertex_data<VId,void,VV>` |

**incidence:**
| New Row | Return |
|---------|--------|
| `for(auto&& [vid] : basic_incidence(g,uid))` | `edge_data<VId,false,void,void>` |
| `for(auto&& [vid, val] : basic_incidence(g,uid,evf))` | `edge_data<VId,false,void,EV>` |

**neighbors:**
| New Row | Return |
|---------|--------|
| `for(auto&& [vid] : basic_neighbors(g,uid))` | `neighbor_data<VId,false,void,void>` |
| `for(auto&& [vid, val] : basic_neighbors(g,uid,vvf))` | `neighbor_data<VId,false,void,VV>` |

**edgelist:**
| New Row | Return |
|---------|--------|
| `for(auto&& [uid, vid] : basic_edgelist(g))` | `edge_data<VId,true,void,void>` |
| `for(auto&& [uid, vid, val] : basic_edgelist(g,evf))` | `edge_data<VId,true,void,EV>` |

Add brief prose explaining that basic views omit descriptors and yield only IDs.

### G.2 — Incoming edge variants

Add new subsections or rows for `in_incidence` and `in_neighbors`:

| View | Concept Required | Return |
|------|-----------------|--------|
| `in_incidence(g, u)` | `bidirectional_adjacency_list` | Same as `incidence` but incoming |
| `basic_in_incidence(g, uid)` | `bidirectional_adjacency_list` | Same as `basic_incidence` but incoming |
| `in_neighbors(g, u)` | `bidirectional_adjacency_list` | Same as `neighbors` but incoming |
| `basic_in_neighbors(g, uid)` | `bidirectional_adjacency_list` | Same as `basic_neighbors` but incoming |

Per strategy Q5 recommendation (b), add a brief exposition-only note about the
`edge_accessor` mechanism that enables these variants.

### G.3 — Transpose view

Add a new subsection in Graph Views for `transpose`:

```latex
\subsection{transpose View}
```

Document `transpose_view<G>` as a zero-cost wrapper for bidirectional graphs that swaps
outgoing/incoming edge traversal. Not a range itself — a graph wrapper. Requires
`bidirectional_adjacency_list<G>`.

### G.4 — Value function concepts and allocator parameter documentation

Add a brief reference to the `vertex_value_function` and `edge_value_function` concepts
(defined in D3130 GCI). These should be mentioned in the Introduction or in a short
"Value Function Concepts" paragraph near the data structs section:

```cpp
template <class VVF, class Graph, class VertexDescriptor>
concept vertex_value_function = /* see D3130 */;

template <class EVF, class Graph, class EdgeDescriptor>
concept edge_value_function = /* see D3130 */;
```

Document the allocator parameter for search views (DFS, BFS, topological sort).
The implementation disambiguates the allocator from value functions using a
`requires(!vertex_value_function<Alloc,...>)` constraint. Add a brief note in the
DFS, BFS, and topological sort subsections showing the allocator overload and
its disambiguation.

### G.5 — Pipe syntax / Range Adaptors

Add pipe syntax examples to each graph view table (via additional example rows or a
separate column). Then add a summary "Range Adaptors" subsection listing all adaptor
objects:

```cpp
g | vertexlist()          g | basic_vertexlist()
g | incidence(uid)        g | basic_incidence(uid)
g | neighbors(uid)        g | basic_neighbors(uid)
g | edgelist()            g | basic_edgelist()
g | vertices_dfs(seed)    g | edges_dfs(seed)
g | vertices_bfs(seed)    g | edges_bfs(seed)
g | vertices_topological_sort()
```

### Procedure

1. Add basic variant rows to each graph view table (G.1).
2. Add `in_` variant subsections/rows (G.2).
3. Add transpose subsection (G.3).
4. Add value function concept references and allocator parameter documentation (G.4).
5. Add pipe syntax examples and summary section (G.5).
6. **Verify:** `latexmk -output-directory=pdf -aux-directory=aux D3129_Views.tex` builds.
7. **Commit:** `"Phase G: Add basic views, in_ variants, transpose, pipe syntax, VVF/EVF concepts"`

### Safety notes

- G.1 modifies the same tables as Phase C. Verify table structure carefully.
- Per Q2 recommendation, if a combined table overflows the page, split basic rows
  into a follow-on table within the same subsection.

---

## Phase H — Introduction, Prose Audit & Cleanup

**Goal:** Systematic cleanup pass through all D3129 text.

### Introduction updates (lines 1–30)

- Remove `\phil{Use \tcode{basic_index_adjacency_list} as the concept for all views.}` —
  concept no longer exists.
- Remove `\phil{Group views by basic first, like with concepts.}` — resolved by basic
  variant placement in Phase G.
- Remove `\phil{Use \tcode{&&} instead of \tcode{const&} for value function parameters.}` —
  implementation uses `VVF&&`/`EVF&&` forwarding references. Note this in prose if
  appropriate.
- Remove `\phil{Can \tcode{std::ref} be used for the value function?}` — moot with 2-arg
  convention.
- Remove or update the commented-out consolidation block (lines 15–22).
- Update prose references to "View Return Types" to reference data structs.

### Search-and-replace pass

| Old term | New term | Scope |
|----------|----------|-------|
| `vertex_info` | `vertex_data` | All remaining occurrences |
| `edge_info` | `edge_data` | All remaining occurrences |
| `neighbor_info` | `neighbor_data` | All remaining occurrences |
| `vertex_reference_t<G>` | `vertex_t<G>` | All remaining occurrences |
| `edge_reference_t<G>` | `edge_t<G>` | All remaining occurrences |
| `vvf(u)` (in prose) | `vvf(g, u)` | All prose references |
| `evf(uv)` (in prose) | `evf(g, uv)` | All prose references |

### Additional cleanup

- Remove all remaining `\phil` notes throughout `views.tex`.
- Verify all `\label` / `\ref` pairs still resolve.
- Verify all `\lstinputlisting` paths are correct.

### Procedure

1. Remove `\phil` notes from introduction.
2. Run search-and-replace for each old term.
3. Remove remaining `\phil` notes throughout the document.
4. Spot-check all `\label`/`\ref` and `\lstinputlisting` references.
5. **Verify:** `latexmk -output-directory=pdf -aux-directory=aux D3129_Views.tex` builds.
6. **Commit:** `"Phase H: Prose audit — cleanup phil notes, rename all obsolete types"`

---

## Phase I — Revision History & Config

**Goal:** Update `revision.tex` and `config.tex` to reflect the r3 changes.

### `config.tex` changes

- `\newcommand{\docno}{\paperno r3}` — already correct (verify only)
- `\newcommand{\prevdocno}{P3129r1}` → update to `P3129r2` (or whatever the actual
  previous published revision is)

### `revision.tex` — add r3 entry

Use the draft entry from strategy §8 (already validated). Ensure it references:
- Data struct rename
- 2-arg value functions
- Basic view variants
- Incoming edge variants
- Transpose view
- Pipe syntax
- `_safe` topological sort factories
- All-vertex topological sort
- Sourced search view removal
- Member functions for search control
- `search_view` concept
- `vertex_t<G>`/`edge_t<G>` descriptor types

### Procedure

1. Update `config.tex` if needed.
2. Add r3 entry to `revision.tex`.
3. **Verify:** `latexmk -output-directory=pdf -aux-directory=aux D3129_Views.tex` builds.
4. **Commit:** `"Phase I: Update revision history and config for D3129r3"`

---

## Phase J — Cross-Paper Fixups

**Goal:** Update references to D3129-defined or D3129-consumed types in shared files,
and update D3126 Overview to note the C++23 dependency introduced by `std::expected`.

### Affected files

| File | Changes |
|------|---------|
| `tex/conventions.tex` | VVF: `vvf(u)` → `vvf(g, u)`. EVF: `evf(uv)` → `evf(g, uv)`. |
| `D3126_Overview/tex/overview.tex` | Add note near §Freestanding: library baseline is C++20; topological sort `_safe` views use `std::expected` (C++23); implementations provide C++20 backward compatibility via external `expected` library. |

**Note:** `conventions.tex` is shared across all papers. Only update the rows that are
already wrong for D3129. If GCI Phase J has already updated descriptor types, verify
those changes are in place rather than re-applying them.

### Procedure

1. Check what GCI Phase J has already done in `conventions.tex`.
2. Update VVF/EVF descriptions if not already done.
3. In `D3126_Overview/tex/overview.tex`, add a "Language Requirements" subsection near
   the existing "Freestanding" section stating:
   - The library targets C++20 as its baseline.
   - The topological sort safe view factories use `std::expected` (C++23).
   - Implementations provide backward compatibility to C++20 via an external `expected`
     library (e.g., `tl::expected`), switching to `std::expected` when C++23 or later
     is used for building.
4. **Verify:** run direct `latexmk` builds for each affected paper to confirm no breakage.
  - `latexmk -output-directory=pdf -aux-directory=aux D3129_Views.tex`
  - `latexmk -output-directory=pdf -aux-directory=aux D3130_Container_Interface.tex` (if `tex/conventions.tex` changed)
  - `latexmk -output-directory=pdf -aux-directory=aux D3128_Algorithms.tex` (if `tex/conventions.tex` changed)
  - `latexmk -output-directory=pdf -aux-directory=aux D3126_Overview.tex`
5. **Commit:** `"Phase J: Cross-paper fixups — conventions.tex VVF/EVF, D3126 C++23 note"`

### Safety notes

- This phase touches shared files. Coordinate with GCI and Algorithms plan status.
- If GCI Phase J is not yet committed, defer this phase until it is.

---

## Phase dependency diagram

```
  Phase A (source files)
    └── Phase B (data structs section)
          ├── Phase C (graph views)
          └── Phase D (search common types)
                ├── Phase E (DFS/BFS) ← also needs C
                └── Phase F (topological sort) ← also needs C

  Phase G (new content: basic, in_, transpose, pipe, VVF/EVF, allocator docs)
    depends on C, D, E, F

All A–G ──→ Phase H (prose audit & cleanup)
               └── Phase I (revision history & config)
                     └── Phase J (cross-paper fixups, D3126 C++23 note)
```

---

## Risk register

| Risk | Likelihood | Impact | Mitigation |
|------|------------|--------|------------|
| `\lstinputlisting` broken by file rename | High (if not careful) | Build failure | Phase A explicitly handles all directives. Verify build after. |
| Table overflow from basic variant rows | Medium | LaTeX formatting issue | Per Q2: split into follow-on table if combined table exceeds page width/height. |
| Bulk rename introduces false positives | Medium | Incorrect prose | Use targeted edits, not blind sed. Spot-check every replacement. |
| Section numbering drift after adding sections | Low | LaTeX warnings | Use `\label`/`\ref` for all cross-references. |
| `conventions.tex` conflict with GCI Phase J | Medium | Git merge conflict | Phase J is last and checks GCI status first. |
| `\phil` note removal misses one | Low | Stale commentary | Run `grep -n '\\phil' views.tex` as final check in Phase H. |
| `std::expected` not available in LaTeX preamble | Low | Undefined `\tcode` | Verify `\tcode{std::expected}` renders correctly. |
| D3126 Overview C++20 language outdated | Medium | Inconsistent version claims | Phase J adds C++23 note to D3126 adjacent to Freestanding section. |
