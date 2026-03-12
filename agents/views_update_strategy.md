# D3129 Views Document Update Strategy

## Purpose

This document captures all discrepancies between the **D3129 Graph Views** LaTeX proposal
(D3129r3, unreleased) and the **graph-v3** reference implementation in
`graph-v3/include/graph/views/` and `graph-v3/include/graph/graph_data.hpp`, along with a
concrete plan to bring the LaTeX document into alignment with the implementation.

**The implementation is the definitive/authoritative source.** Where the LaTeX and the
implementation disagree, the implementation is correct and the LaTeX must be updated.

### Relationship to Other Strategy Documents

- **`gci_update_strategy.md`** defines the fundamental descriptor shift
  (`vertex_reference_t<G>` → `vertex_t<G>`, `edge_reference_t<G>` → `edge_t<G>`) and the
  `vertex_data`/`edge_data`/`neighbor_data` type renames that originate in D3130. The Views
  paper *consumes* those types — it does not define them. This strategy applies the
  GCI-defined types to the Views paper.
- **`algo_update_strategy.md`** defines the visitor and weight function convention changes
  (`(graph, descriptor)` 2-arg form). The Views paper uses the same 2-arg convention for
  value functions, paralleling the algorithm convention.
- Both companion strategies are complete (GCI: committed through Phase J; Algorithms: plan
  written). This strategy is the third leg of the update triangle.

### Scope

| In scope | Out of scope |
|----------|-------------|
| `D3129_Views/tex/views.tex` — all sections | D3130 GCI paper (handled by `gci_update_strategy.md`) |
| `D3129_Views/src/*.hpp` — all source files | D3128 Algorithms paper (handled by `algo_update_strategy.md`) |
| `D3129_Views/tex/revision.tex` — revision history | Shared tex files (`conventions.tex`, `getting_started.tex`) — separate cross-paper fixup |
| `D3129_Views/tex/config.tex` — version bump | graph-v3 implementation changes |

---

## 0. Fundamental Design Shifts Affecting Views

Three design shifts from the implementation affect every section of D3129:

### 0.1 Info Structs → Data Structs

The most pervasive change is the **rename and redesign** of the three core return types:

| Old (LaTeX D3129r3) | New (Implementation) | File |
|---------------------|---------------------|------|
| `vertex_info<VorVId, VV>` | `vertex_data<VId, V, VV>` | `graph_data.hpp` |
| `edge_info<VorVId, Sourced, E, EV>` | `edge_data<VId, Sourced, E, EV>` | `graph_data.hpp` |
| `neighbor_info<VorVId, Sourced, VV>` | `neighbor_data<VId, Sourced, V, VV>` | `graph_data.hpp` |

**Key structural differences:**

1. **Three template parameters → more template parameters.** `vertex_info<VorVId, VV>` had
   2 parameters where `VorVId` was overloaded to mean either a vertex descriptor or a vertex
   id. `vertex_data<VId, V, VV>` has 3 parameters with separate `VId` (vertex id type) and
   `V` (vertex descriptor type), each independently voidable.

2. **8 specializations for vertex_data** (vs 2 for vertex_info). The void-combination space
   for `{VId, V, VV}` yields 8 specializations (2³), each eliminating its void fields from
   the struct. The LaTeX only has `{VorVId, VV}` with 2 specializations.

3. **16 specializations for edge_data** (in the main template). The void-combination space
   for `{VId, Sourced, E, EV}` with VId=void yields additional specializations for
   descriptor-based views where no vertex IDs are available.

4. **`neighbor_data` gains a vertex descriptor parameter `V`.** The old `neighbor_info<VorVId, Sourced, VV>`
   becomes `neighbor_data<VId, Sourced, V, VV>` — adding explicit support for the target
   vertex descriptor, not just its id.

5. **Member names changed.** `vertex_info::vertex` → `vertex_data::vertex` (same) but
   `vertex_data::id` is new (replaces implicit overloading). `edge_info::source` →
   `edge_data::source_id`, `edge_info::target` → `edge_data::target_id`, `edge_info::edge`
   → `edge_data::edge` (same).

### 0.2 Value Functions: 1-arg → 2-arg

| Aspect | LaTeX (D3129r3) | Implementation |
|--------|-----------------|---------------|
| Vertex value function | `vvf(u)` — 1-arg, descriptor only | `vvf(g, u)` — 2-arg, `(const G&, vertex_t<G>)` |
| Edge value function | `evf(uv)` — 1-arg, edge descriptor only | `evf(g, uv)` — 2-arg, `(const G&, edge_t<G>)` |
| Value function concept | Not in LaTeX | `vertex_value_function<VVF, G, V>`, `edge_value_function<EVF, G, E>` |

The 2-arg form enables stateless lambdas (no capture needed — the graph is a parameter),
which satisfies `semiregular` and enables `std::views` chaining.

**Impact:** Every code example in the paper that shows `vvf(u)` or `evf(uv)` must change
to `vvf(g, u)` or `evf(g, uv)`. The lambda captures `[&g]` become `[]`.

### 0.3 Reference Types → Descriptor Types

| Old (LaTeX) | New (Implementation) |
|-------------|---------------------|
| `vertex_reference_t<G>` | `vertex_t<G>` (descriptor, value type) |
| `edge_reference_t<G>` | `edge_t<G>` (descriptor, value type) |

All code examples using `vertex_reference_t<G>` must change to `vertex_t<G>`.

### 0.4 Basic Variants (New)

The implementation introduces a `basic_` prefix variant for every non-search view. Basic
views omit the descriptor from the structured binding, yielding only IDs (and optional
value). This is **not in the current LaTeX** at all:

| Standard View | Basic View | Standard Binding | Basic Binding |
|--------------|-----------|-----------------|---------------|
| `vertexlist(g)` | `basic_vertexlist(g)` | `[uid, u]` | `[uid]` |
| `vertexlist(g, vvf)` | `basic_vertexlist(g, vvf)` | `[uid, u, val]` | `[uid, val]` |
| `incidence(g, u)` | `basic_incidence(g, uid)` | `[tid, uv]` | `[tid]` |
| `incidence(g, u, evf)` | `basic_incidence(g, uid, evf)` | `[tid, uv, val]` | `[tid, val]` |
| `neighbors(g, u)` | `basic_neighbors(g, uid)` | `[tid, n]` | `[tid]` |
| `neighbors(g, u, vvf)` | `basic_neighbors(g, uid, vvf)` | `[tid, n, val]` | `[tid, val]` |
| `edgelist(g)` | `basic_edgelist(g)` | `[sid, tid, uv]` | `[sid, tid]` |
| `edgelist(g, evf)` | `basic_edgelist(g, evf)` | `[sid, tid, uv, val]` | `[sid, tid, val]` |

Basic views are lighter weight — no descriptor materialization. They are the preferred
form when the algorithm only needs vertex/edge IDs.

### 0.5 Sourced Variants Removed from Search Views

The LaTeX has `sourced_edges_dfs`, `sourced_edges_bfs`, and `sourced_edges_topological_sort`
variants. **These do not exist in the implementation.** The implementation's edge search
views return `edge_data<void, false, edge_t<G>, EV>` — unsourced, with just the edge
descriptor (no vertex IDs).

| LaTeX | Implementation | Status |
|-------|---------------|--------|
| `sourced_edges_dfs(g, source)` → `{u, v, uv}` | Not present | **Remove** |
| `sourced_edges_bfs(g, source)` → `{u, v, uv}` | Not present | **Remove** |
| `sourced_edges_topological_sort(g, source)` → `{u, v, uv}` | Not present | **Remove** |

### 0.6 Search Views: Descriptor-Based Return (No Vertex IDs)

The LaTeX search views include vertex IDs in structured bindings — e.g.,
`vertices_dfs(g,source)` → `vertex_info<void,V,void>` → `[v]` where `v` is a descriptor.
The implementation returns `vertex_data<void, vertex_t<G>, void>` — VId is `void`, so
there is no vertex ID in the binding, only the descriptor. Users can obtain the ID via
`vertex_id(g, v)`.

Similarly, edge search views return `edge_data<void, false, edge_t<G>, void>` — no
source_id, no target_id (both void), just the edge descriptor.

This is a deliberate design: search views operate on graph topology via descriptors, not IDs.

### 0.7 Incoming Edge Variants (New)

The implementation adds `in_` prefixed variants for per-vertex views via the `in_edge_accessor`
template parameter:

| New View | Concept Required | Equivalent To |
|---------|-----------------|---------------|
| `in_incidence(g, u)` | `bidirectional_adjacency_list` | `incidence` but traverses incoming edges |
| `basic_in_incidence(g, uid)` | `bidirectional_adjacency_list` | `basic_incidence` for incoming edges |
| `in_neighbors(g, u)` | `bidirectional_adjacency_list` | `neighbors` but returns source vertices |
| `basic_in_neighbors(g, uid)` | `bidirectional_adjacency_list` | `basic_neighbors` for incoming edges |

These are **not in the current LaTeX**.

### 0.8 Pipe Syntax / Range Adaptors (New)

The implementation provides pipe-syntax adaptor objects in `graph::views::adaptors`
(via `adaptors.hpp`). Every view supports `g | view_name(args...)` syntax:

```cpp
for (auto [uid, u] : g | vertexlist())          { ... }
for (auto [tid, e] : g | incidence(uid))        { ... }
for (auto [v]      : g | vertices_dfs(seed))    { ... }
for (auto [uv]     : g | edges_bfs(seed, evf))  { ... }
```

This is **not in the current LaTeX**.

---

## 1. Section-by-Section Discrepancies

### 1.1 Introduction (lines 1–30 of views.tex)

| Issue | Action |
|-------|--------|
| `\phil` notes reference `basic_index_adjacency_list` | Remove — concept no longer exists. Views use `adjacency_list` or `index_adjacency_list`. |
| `\phil` note "Group views by basic first" | Resolve: the implementation has `basic_` variants. Decide on presentation order. |
| `\phil` note "Use `&&` instead of `const&` for value function parameters" | The implementation uses `VVF&&` / `EVF&&` (forwarding references). Document this. |
| `\phil` note "Can `std::ref` be used for the value function?" | Resolve or remove. The 2-arg value function convention makes this moot — stateless lambdas don't need `ref`. |
| Prose references "View Return Types" | Update to reference `vertex_data`, `edge_data`, `neighbor_data`. |
| Old comment block about consolidating views (lines 15–22) | Review for relevance; delete or update. The `basic_` split in the implementation addresses this. |

### 1.2 Info Structs Section (lines 31–195)

This is the "Info Structs (Return Types)" section. It needs a complete rewrite.

#### 1.2.1 `vertex_info` → `vertex_data`

| Aspect | LaTeX | Implementation | Change |
|--------|-------|---------------|--------|
| Name | `vertex_info<VorVId, VV>` | `vertex_data<VId, V, VV>` | Rename + add V parameter |
| Template params | 2 (`VorVId`, `VV`) | 3 (`VId`, `V`, `VV`) | Add vertex descriptor param |
| Specializations | 2 (VV=void) | 8 (all {VId,V,VV} void combos) | Expand table |
| Member `vertex` comment | "e.g. vertex_reference_t<G>" | "e.g. vertex_t<G> (vertex descriptor)" | Update type reference |
| Source file | `vertex_info.hpp` | N/A (defined in `graph_data.hpp`) | Rewrite source file |

#### 1.2.2 `edge_info` → `edge_data`

| Aspect | LaTeX | Implementation | Change |
|--------|-------|---------------|--------|
| Name | `edge_info<VorVId, Sourced, E, EV>` | `edge_data<VId, Sourced, E, EV>` | Rename |
| Member names | `source`, `target` | `source_id`, `target_id` | Rename members |
| Member type names | `source_type`, `target_type` | `source_id_type`, `target_id_type` | Rename type aliases |
| `edge_type` comment | "e.g. edge_reference_t<G>" | "e.g. edge_t<G> (edge descriptor)" | Update |
| Source file | `edge_info.hpp` | N/A (defined in `graph_data.hpp`) | Rewrite source file |

#### 1.2.3 `neighbor_info` → `neighbor_data`

| Aspect | LaTeX | Implementation | Change |
|--------|-------|---------------|--------|
| Name | `neighbor_info<VorVId, Sourced, VV>` | `neighbor_data<VId, Sourced, V, VV>` | Rename + add V parameter |
| Template params | 3 (`VorVId`, `Sourced`, `VV`) | 4 (`VId`, `Sourced`, `V`, `VV`) | Add vertex descriptor param |
| Member names | `source`, `target` | `source_id`, `target_id`, `target` (vertex descriptor) | Split target from target_id |
| Source file | `neighbor_info.hpp` | N/A (defined in `graph_data.hpp`) | Rewrite source file |

#### 1.2.4 Code Examples

All introductory code examples (lines 39–78 of views.tex) need updating:

| Old Code | New Code |
|----------|----------|
| `vertex_reference_t<G> u = uu.vertex;` | `vertex_t<G> u = uu.vertex;` |
| `auto vvf = [&g](vertex_reference_t<G> u) { return vertex_value(g,u); };` | `auto vvf = [](const auto& g, vertex_t<G> u) { return vertex_value(g,u); };` |
| `// type is vertex_info<vertex_t<G>, void>` | `// type is vertex_data<vertex_id_t<G>, vertex_t<G>, void>` |
| `for(auto&& uu : vertexlist(g))` — `uu.vertex` | `for(auto&& [uid, u] : vertexlist(g))` — now yields id + descriptor |
| `for(auto&& [u] : vertexlist(g))` | `for(auto&& [uid, u] : vertexlist(g))` — two bindings now |
| `for(auto&& [u, value] : vertexlist(g, vvf))` | `for(auto&& [uid, u, val] : vertexlist(g, vvf))` — three bindings now |

**Critical change:** In the old design, `vertexlist(g)` yielded `[u]` (just the descriptor).
In the implementation, `vertexlist(g)` yields `[uid, u]` (vertex id + descriptor). This is
because `vertex_data` always includes the id when VId≠void.

#### 1.2.5 Copyable Info Structs (Commented-Out Section)

Lines ~200–240 contain a `\begin{comment}...\end{comment}` block about copyable info
structs. The implementation has `copyable_vertex_t<VId, VV>`, `copyable_edge_t<VId, EV>`,
and `copyable_neighbor_t<VId, VV>` type aliases. **Decision needed**: un-comment and update,
or leave out. The copyable aliases are used by algorithms and views internally.

---

### 1.3 Graph Views — vertexlist (lines ~245–270)

| Aspect | LaTeX | Implementation | Change |
|--------|-------|---------------|--------|
| Concept | Not specified | `adjacency_list` (descriptor overloads), `index_adjacency_list` (iterator/id overloads) | Document |
| `vertexlist(g)` return | `vertex_info<void,V,void>` → `[u]` | `vertex_data<VId,V,void>` → `[uid, u]` | **Breaking**: now 2 bindings |
| `vertexlist(g,vvf)` return | `vertex_info<void,V,VV>` → `[u,val]` | `vertex_data<VId,V,VV>` → `[uid, u, val]` | **Breaking**: now 3 bindings |
| VVF convention | `vvf(u)` — 1-arg | `vvf(g, u)` — 2-arg | Update |
| `basic_vertexlist(g)` | **Not in LaTeX** | `vertex_data<VId,void,void>` → `[uid]` | **Add** |
| `basic_vertexlist(g,vvf)` | **Not in LaTeX** | `vertex_data<VId,void,VV>` → `[uid, val]` | **Add** |
| Iterator/subrange overloads | `vertexlist(g,first,last)`, `vertexlist(g,vr)` | Same pattern, plus basic variants | Update + add basic |
| Iterator type | Not specified | `forward_iterator`, sized, common range | Document |
| Pipe syntax | Not in LaTeX | `g \| vertexlist()`, `g \| basic_vertexlist()` | **Add** |

### 1.4 Graph Views — incidence (lines ~272–295)

| Aspect | LaTeX | Implementation | Change |
|--------|-------|---------------|--------|
| `incidence(g,u)` return | `edge_info<void,false,E,void>` → `[uv]` | `edge_data<VId,false,E,void>` → `[tid, uv]` | **Breaking**: now 2 bindings |
| `incidence(g,u,evf)` return | `edge_info<void,false,E,EV>` → `[uv,val]` | `edge_data<VId,false,E,EV>` → `[tid, uv, val]` | **Breaking**: now 3 bindings |
| EVF convention | `evf(uv)` — 1-arg | `evf(g, uv)` — 2-arg | Update |
| `basic_incidence(g,uid)` | **Not in LaTeX** | `edge_data<VId,false,void,void>` → `[tid]` | **Add** |
| `basic_incidence(g,uid,evf)` | **Not in LaTeX** | `edge_data<VId,false,void,EV>` → `[tid, val]` | **Add** |
| `in_incidence(g,u)` | **Not in LaTeX** | Same types, incoming edges | **Add** (requires `bidirectional_adjacency_list`) |
| `basic_in_incidence(g,uid)` | **Not in LaTeX** | Same as basic, incoming | **Add** |
| "Since the source vertex `u` is available..." prose | Present | Still true — source is implicit | Keep but update references |
| Pipe syntax | Not in LaTeX | `g \| incidence(uid)` | **Add** |

### 1.5 Graph Views — neighbors (lines ~297–318)

| Aspect | LaTeX | Implementation | Change |
|--------|-------|---------------|--------|
| `neighbors(g,uid)` return | `neighbor_info<void,false,V,void>` → `[v]` | `neighbor_data<VId,false,V,void>` → `[tid, n]` | **Breaking**: now 2 bindings |
| `neighbors(g,uid,vvf)` return | `neighbor_info<void,false,V,VV>` → `[v,val]` | `neighbor_data<VId,false,V,VV>` → `[tid, n, val]` | **Breaking**: now 3 bindings |
| Value function | `vvf(u)` — 1-arg | `vvf(g, u)` — 2-arg, vertex_value_function | Update |
| `basic_neighbors(g,uid)` | **Not in LaTeX** | `neighbor_data<VId,false,void,void>` → `[tid]` | **Add** |
| `basic_neighbors(g,uid,vvf)` | **Not in LaTeX** | `neighbor_data<VId,false,void,VV>` → `[tid, val]` | **Add** |
| `in_neighbors(g,u)` | **Not in LaTeX** | Same types, incoming direction | **Add** |
| `basic_in_neighbors(g,uid)` | **Not in LaTeX** | Same as basic, incoming | **Add** |
| Pipe syntax | Not in LaTeX | `g \| neighbors(uid)` | **Add** |

### 1.6 Graph Views — edgelist (lines ~320–340)

| Aspect | LaTeX | Implementation | Change |
|--------|-------|---------------|--------|
| `edgelist(g)` return | `edge_info<V,true,E,void>` → `[u,v,uv]` | `edge_data<VId,true,E,void>` → `[sid, tid, uv]` | Binding names change; VId replaces V |
| `edgelist(g,evf)` return | `edge_info<V,true,E,EV>` → `[u,v,uv,val]` | `edge_data<VId,true,E,EV>` → `[sid, tid, uv, val]` | Same pattern |
| EVF convention | `evf(uv)` — 1-arg | `evf(g, uv)` — 2-arg | Update |
| `basic_edgelist(g)` | **Not in LaTeX** | `edge_data<VId,true,void,void>` → `[sid, tid]` | **Add** |
| `basic_edgelist(g,evf)` | **Not in LaTeX** | `edge_data<VId,true,void,EV>` → `[sid, tid, val]` | **Add** |
| Edgelist concept reference | `basic_sourced_index_edgelist` | Same | Verify alignment with GCI |
| Iterator type | Not specified | `forward_iterator`, conditionally sized | Document |
| Pipe syntax | Not in LaTeX | `g \| edgelist()` | **Add** |

### 1.7 "Search" Views — Common Types (lines ~343–395)

| Aspect | LaTeX | Implementation | Change |
|--------|-------|---------------|--------|
| `cancel_search` enum | `cancel_search : int8_t { continue_search, cancel_branch, cancel_all }` | Same definition in `search_base.hpp` | Verify alignment |
| `cancel(search, cancel_search)` | Free function `cancel(S, cancel_search)` | Member function `search.cancel(cancel_search)` | **Change to member** |
| `depth(search)` | Free function `depth(S)` → integral | Member function `search.depth()` → `size_t` | **Change to member** |
| `size(search)` | Free function `size(S)` → integral | Member `search.num_visited()` → `size_t` | **Rename**: `size` → `num_visited` |
| `search_view` concept | Not in LaTeX | `requires { v.cancel(); cv.depth(); cv.num_visited(); }` | **Add** |
| `visited_tracker` | Not in LaTeX | Exposition-only helper class | Consider mentioning |
| Code example (lines 390–402) | Uses `depth(dfs)`, `cancel(dfs, ...)`, `size(dfs)` | Use `dfs.depth()`, `dfs.cancel(...)`, `dfs.num_visited()` | Update |
| Prose about `size(dfs)` vs `depth(dfs)` | Present | Replace `size` with `num_visited` in prose | Update |
| Prose about edgelist concept for sourced views | References sourced view producing edgelist-compatible ranges | Sourced search views removed — delete this prose | **Remove** |

### 1.8 DFS Views (lines ~405–440)

| Aspect | LaTeX | Implementation | Change |
|--------|-------|---------------|--------|
| `vertices_dfs(g,source)` return | `vertex_info<void,V,void>` → `[v]` | `vertex_data<void,V,void>` → `[v]` | Names differ but binding compatible |
| `vertices_dfs(g,source,vvf)` return | `vertex_info<void,V,VV>` → `[v,val]` | `vertex_data<void,V,VV>` → `[v, val]` | Update type name |
| `edges_dfs(g,source)` return | `edge_info<V,false,E,void>` → `[v,uv]` | `edge_data<void,false,E,void>` → `[uv]` | **Breaking**: was `[v,uv]`, now `[uv]` only |
| `edges_dfs(g,source,evf)` return | `edge_info<V,false,E,EV>` → `[v,uv,val]` | `edge_data<void,false,E,EV>` → `[uv, val]` | **Breaking**: was `[v,uv,val]`, now `[uv, val]` |
| `sourced_edges_dfs(g,source)` | `edge_info<V,true,E,void>` → `[u,v,uv]` | **Does not exist** | **Remove** |
| `sourced_edges_dfs(g,source,evf)` | `edge_info<V,true,E,EV>` → `[u,v,uv,val]` | **Does not exist** | **Remove** |
| VVF/EVF convention | 1-arg | 2-arg | Update |
| Seed parameter type | `source` (vertex_id) | `seed` as either `vertex_id_t<G>` or `vertex_t<G>` | Update — implementation accepts both |
| Allocator parameter | "Removed" per r0 revision, then "Restored" per r1 | Present in implementation, disambiguated from VVF via concept | Ensure allocator overloads documented |
| Iterator type | Not specified | `input_iterator`, single-pass, sentinel-based | Document |
| `\phil` notes about event enum bitset | Present | Not in implementation | Remove or move to future work |
| Pipe syntax | Not in LaTeX | `g \| vertices_dfs(seed)` | **Add** |

### 1.9 BFS Views (lines ~450–487)

Identical structural changes as DFS (§1.8):

| Aspect | LaTeX | Implementation | Change |
|--------|-------|---------------|--------|
| `vertices_bfs` | Same changes as `vertices_dfs` | Same | Apply same updates |
| `edges_bfs(g,source)` | `[v,uv]` | `[uv]` | **Breaking**: descriptor only |
| `sourced_edges_bfs(...)` | Present | **Does not exist** | **Remove** |
| `\phil` note about `depth_limit` parameter | Present | Not in implementation | Remove or move to future work |
| `\phil` notes about event enum bitset | Present | Not in implementation | Remove |
| Pipe syntax | Not in LaTeX | Present in implementation | **Add** |

### 1.10 Topological Sort Views (lines ~489–522)

| Aspect | LaTeX | Implementation | Change |
|--------|-------|---------------|--------|
| Seed parameter | `source` — single source vertex | **No seed** — processes all vertices | **Breaking change** |
| `vertices_topological_sort(g,source)` | 2-arg | `vertices_topological_sort(g)` — 1-arg | Remove source param |
| `edges_topological_sort(g,source)` | 2-arg | `edges_topological_sort(g)` — 1-arg | Remove source param |
| `sourced_edges_topological_sort(...)` | Present | **Does not exist** | **Remove** |
| `_safe` cycle-detection factories | **Not in LaTeX** | Returns `tl::expected<view, vertex_t<G>>` | **Add** |
| Iterator type | Not specified | **`forward_iterator`** (eagerly computed) | Document — differs from DFS/BFS |
| `depth()` member | Implied by "search views" section | **Not available** — topo sort has no tree depth | Clarify: topo sort does NOT satisfy `search_view` |
| `cancel()` member | Implied | Present, but `cancel_branch` = `cancel_all` (flat ordering) | Document |
| Pipe syntax | Not in LaTeX | `g \| vertices_topological_sort()` | **Add** |

---

## 2. New Content Not in LaTeX

### 2.1 Transpose View

The implementation has `transpose_view<G>` (in `views/transpose.hpp`) — a zero-cost wrapper
for bidirectional graphs that swaps outgoing/incoming edge traversal:

```cpp
auto tv = transpose(g); // transpose_view<G>
// edges(tv, v) → in_edges(g, v)
// in_edges(tv, v) → edges(g, v)
// target_id(tv, e) → source_id(g, e) and vice versa
```

Requires `bidirectional_adjacency_list<G>`. Not a range itself — used as a graph wrapper
passed to other views and algorithms.

**Action:** Add a new subsection for `transpose` in the Graph Views section.

### 2.2 Edge Accessor Pattern

The implementation parameterizes per-vertex views (`incidence`, `neighbors`, BFS, DFS,
topological sort) on an `Accessor` template parameter:

| Accessor | `edges()` call | `neighbor_id()` call | Constraint |
|----------|---------------|---------------------|-----------|
| `out_edge_accessor` (default) | `out_edges(g, u)` | `target_id(g, e)` | `adjacency_list` |
| `in_edge_accessor` | `in_edges(g, u)` | `source_id(g, e)` | `bidirectional_adjacency_list` |

This is the mechanism behind the `in_incidence`, `in_neighbors` etc. factory functions.

**Action:** Decide whether to expose the accessor pattern in the paper (as exposition-only
infrastructure) or simply document the `in_` factory function variants without explaining
the mechanism.

### 2.3 View Concepts

The implementation defines a `search_view` concept:

```cpp
template <class V>
concept search_view = requires(V& v, const V& cv) {
    { v.cancel() } -> std::convertible_to<cancel_search>;
    { cv.depth() } -> std::convertible_to<std::size_t>;
    { cv.num_visited() } -> std::convertible_to<std::size_t>;
};
```

Satisfied by DFS and BFS views but **NOT** by topological sort (which has no `depth()`).

**Action:** Add `search_view` concept to the Common Types section. Document which views
satisfy it and which do not.

### 2.4 Value Function Concepts

The implementation defines formal concepts for value functions:

```cpp
template <class VVF, class Graph, class VertexDescriptor>
concept vertex_value_function =
    std::invocable<VVF, const Graph&, VertexDescriptor> &&
    (!std::is_void_v<std::invoke_result_t<VVF, const Graph&, VertexDescriptor>>);

template <class EVF, class Graph, class EdgeDescriptor>
concept edge_value_function =
    std::invocable<EVF, const Graph&, EdgeDescriptor> &&
    (!std::is_void_v<std::invoke_result_t<EVF, const Graph&, EdgeDescriptor>>);
```

These are used in view factory function constraints and are defined in D3130 (GCI). The
Views paper should reference them.

**Action:** Reference `vertex_value_function` and `edge_value_function` from D3130, and
show their use in view constraints.

### 2.5 Allocator Disambiguation

Search view factories accept an optional allocator parameter that is disambiguated from
value functions using a `requires(!vertex_value_function<Alloc,...>)` constraint:

```cpp
template <index_adjacency_list G, class Alloc>
  requires(!vertex_value_function<Alloc, G, vertex_t<G>>)
auto vertices_dfs(G& g, vertex_id_t<G> seed, Alloc alloc);
```

**Action:** Document the allocator parameter and its disambiguation. This was "restored"
in r1 but currently under-specified in the prose.

### 2.6 Pipe Syntax (Range Adaptors)

All views support C++20 pipe syntax via adaptor objects:

```cpp
// Basic views
for (auto [uid, u]      : g | vertexlist())          { ... }
for (auto [tid, uv]     : g | incidence(uid))        { ... }
for (auto [tid, n]      : g | neighbors(uid))        { ... }
for (auto [sid, tid, uv]: g | edgelist())             { ... }

// Search views
for (auto [v]   : g | vertices_dfs(seed))             { ... }
for (auto [uv]  : g | edges_bfs(seed))                { ... }
for (auto [v]   : g | vertices_topological_sort())    { ... }

// Chaining with standard range adaptors
for (auto id : g | vertexlist()
                 | std::views::transform(...)
                 | std::views::filter(...))           { ... }
```

**Action:** Add a new subsection documenting pipe syntax support and adaptor objects.

---

## 3. Source File Updates

### 3.1 Files to Delete and Rewrite

| Old File | Action | New Content |
|----------|--------|-------------|
| `D3129_Views/src/vertex_info.hpp` | **Rewrite** → `vertex_data.hpp` | `vertex_data<VId, V, VV>` primary template + key specializations |
| `D3129_Views/src/edge_info.hpp` | **Rewrite** → `edge_data.hpp` | `edge_data<VId, Sourced, E, EV>` primary template + key specializations |
| `D3129_Views/src/neighbor_info.hpp` | **Rewrite** → `neighbor_data.hpp` | `neighbor_data<VId, Sourced, V, VV>` primary template + key specializations |

**Note:** The full implementation has many specializations (8 for vertex_data, 16+ for
edge_data). For the paper, show the primary template and a representative subset of
specializations. Use exposition-only commentary for the pattern.

### 3.2 New Source Files to Add

| File | Content | Used By |
|------|---------|---------|
| `copyable_types.hpp` | `copyable_vertex_t`, `copyable_edge_t`, `edgelist_edge` | Multiple sections |
| `cancel_search.hpp` | `cancel_search` enum | Common search types section |

**Decision needed:** Whether to add separate source files for these or inline them in the
LaTeX. The GCI strategy used `\lstinputlisting` for all code; maintain consistency.

---

## 4. Cross-Paper References

### 4.1 References to D3130 (GCI)

The Views paper references GCI-defined types. These references need updating:

| Old Reference | New Reference |
|--------------|--------------|
| `vertex_reference_t<G>` | `vertex_t<G>` |
| `edge_reference_t<G>` | `edge_t<G>` |
| `vertex_info` | `vertex_data` (defined in D3130) |
| `edge_info` | `edge_data` (defined in D3130) |
| `neighbor_info` | `neighbor_data` (defined in D3130) |
| `edges(g,u)` | `out_edges(g,u)` (with `edges` as alias) |
| `degree(g,u)` | `out_degree(g,u)` (with `degree` as alias) |
| `basic_adjacency_list` | **Removed** — use `adjacency_list` |
| `basic_index_adjacency_list` | **Removed** — use `index_adjacency_list` |
| `basic_sourced_adjacency_list` | **Removed** |
| `sourced_adjacency_list` | **Removed** |

### 4.2 References to D3128 (Algorithms)

The search views overlap with algorithm concerns:

| Aspect | Action |
|--------|--------|
| DFS/BFS visitor events vs search view events | Cross-reference D3128 visitor events. Note that search views are an alternative to visitor-based traversal. |
| Topological sort algorithm vs view | Cross-reference D3128's `topological_sort()` algorithm. Note the algorithm returns `bool` for cycle detection; the view offers a `_safe` factory returning `expected`. |

### 4.3 Shared `conventions.tex` Updates

The conventions table (`tex/conventions.tex`) contains entries that apply to views:

| Entry | Current | Needed Update |
|-------|---------|---------------|
| VVF description | `vvf(u)` → vertex value | `vvf(g, u)` → vertex value (2-arg) |
| EVF description | `evf(uv)` → edge value | `evf(g, uv)` → edge value (2-arg) |
| `graph_reference_t<G>` row | Present | Remove (not in implementation) |
| `vertex_edge_range_t<G>` for ER | Present | Update to note `out_edge_range_t<G>` as primary |

**Note:** `conventions.tex` is shared across all papers and should be updated in a
cross-paper fixup phase (like Phase J in the GCI plan).

---

## 5. Open Questions

### Q1 — Specialization Presentation

**Question:** The implementation has 8 `vertex_data` specializations, 16+ `edge_data`
specializations, and 12+ `neighbor_data` specializations. How many should the paper show?

**Options:**
- (a) Show all specializations exhaustively (accurate but verbose)
- (b) Show primary template + key specializations used by views (the LaTeX's current
  approach with a members table)
- (c) Show primary template only with a prose explanation of the void-elimination pattern

**Recommendation:** (b) — matches the existing approach and keeps the paper readable.
Show ~4 key specializations per type (the ones views actually produce), reference the
implementation for the complete set.

### Q2 — Basic View Presentation

**Question:** Should `basic_` variants be presented alongside or separately from standard views?

**Options:**
- (a) Each view subsection shows both standard and basic variants in the same table
- (b) Group all basic views in a separate section
- (c) Show standard views first, then a single "Basic View Variants" subsection listing all

**Decision:** (a) — keeps related information together. Use a horizontal divider
in each view's table to separate standard from basic rows, provided the table fits
on the page. If a combined table is too wide or tall, split basic variants into a
separate follow-on table within the same subsection.

### Q3 — Pipe Syntax Presentation

**Question:** How should pipe syntax be presented?

**Options:**
- (a) A single "Range Adaptors" subsection listing all adaptor objects
- (b) Each view's subsection shows its pipe syntax alongside the function call syntax
- (c) Both — show pipe in each subsection with a summary section

**Decision:** (c) — show pipe syntax in each view table via an additional column or
examples, plus a summary "Range Adaptors" section listing all adaptor objects.

### Q4 — `tl::expected` Dependency

**Question:** The topological sort `_safe` factories return `tl::expected<view, vertex_t<G>>`.
The standard has `std::expected` since C++23. Should the paper:

**Options:**
- (a) Use `std::expected` (C++23 feature)
- (b) Use an exposition-only `expected` type
- (c) Use a different error reporting mechanism (e.g., `optional<vertex_t<G>>` error output)

**Decision:** (b) — use an exposition-only `expected` type to avoid a hard C++23
dependency. This keeps the paper compatible with C++20 while conveying the intended
semantics.

### Q5 — Incoming Edge Search Views

**Question:** The implementation parameterizes BFS, DFS, and topo sort on an `Accessor` but
does not expose `in_vertices_dfs`, `in_edges_bfs`, etc. factory functions. Should the paper
document the accessor mechanism and/or request incoming-edge search variants?

**Options:**
- (a) Document only what exists (outgoing search views only)
- (b) Document the accessor mechanism as exposition-only and note that incoming variants
  are possible future extensions
- (c) Add incoming search view factories (requires implementation work)

**Decision:** (b) — document the accessor mechanism as exposition-only and note that
incoming variants are possible future extensions.

### Q6 — Event Enum / Coroutine-Style Events

**Question:** Multiple `\phil` notes suggest adding an event enum bitset to search views for
coroutine-style event reporting. The implementation does not have this.

**Options:**
- (a) Remove all `\phil` notes about event enums
- (b) Add a "Future Directions" note
- (c) Implement the feature first

**Decision:** (a) — remove all `\phil` notes about event enums. This is a future
direction that can be proposed separately. Keep the current visitor-based approach
(D3128) and search-view approach (D3129) as complementary patterns.

### Q7 — Search View Binding Fields

**Question:** Search views in the implementation return `vertex_data<void, V, void>` for
vertex views (binding: `[v]`) and `edge_data<void, false, E, void>` for edge views
(binding: `[uv]`). The VId is `void` — no vertex id in the binding. Should the paper
explicitly document this design choice and the rationale?

**Decision:** Yes — add a brief "Design Note" paragraph explaining that search views
yield descriptors rather than IDs, because the search context is per-graph (the user already
knows which graph) and descriptors provide richer access via CPOs (`vertex_id(g, v)`,
`vertex_value(g, v)`, etc.).

---

## 6. Implementation Inventory

### 6.1 View File → LaTeX Section Mapping

| Implementation File | Purpose | LaTeX Section |
|--------------------|---------|---------------|
| `views/vertexlist.hpp` | Vertex iteration | §3 vertexlist Views |
| `views/incidence.hpp` | Per-vertex edge iteration | §3 incidence Views |
| `views/neighbors.hpp` | Per-vertex neighbor iteration | §3 neighbors Views |
| `views/edgelist.hpp` | All-edges iteration | §3 edgelist Views |
| `views/dfs.hpp` | DFS vertex/edge traversal | §4 DFS Views |
| `views/bfs.hpp` | BFS vertex/edge traversal | §4 BFS Views |
| `views/topological_sort.hpp` | Topological ordering | §4 Topological Sort Views |
| `views/search_base.hpp` | Shared search infrastructure | §4 Common Types |
| `views/view_concepts.hpp` | `search_view` concept | §4 Common Types |
| `views/transpose.hpp` | Direction-swapping adaptor | New §3 or §5 |
| `views/edge_accessor.hpp` | Direction parameterization | Exposition-only |
| `views/adaptors.hpp` | Pipe syntax support | New §5 or integrated |
| `views/basic_views.hpp` | Convenience header | Not a separate section |
| `graph_data.hpp` | `vertex_data`, `edge_data`, `neighbor_data` | §2 Data Structs |

### 6.2 Test Coverage Reference

The implementation has comprehensive tests that can be used to verify behavior:

| Test File | Validates |
|-----------|----------|
| `tests/views/test_basic_views.cpp` | Basic view factories |
| `tests/views/test_incidence.cpp` | Incidence view |
| `tests/views/test_basic_incidence.cpp` | Basic incidence view |
| `tests/views/test_neighbors.cpp` | Neighbors view |
| `tests/views/test_basic_neighbors.cpp` | Basic neighbors view |
| `tests/views/test_edgelist.cpp` | Edgelist view |
| `tests/views/test_basic_edgelist.cpp` | Basic edgelist view |
| `tests/views/test_dfs.cpp` | DFS search views |
| `tests/views/test_bfs.cpp` | BFS search views |
| `tests/views/test_topological_sort.cpp` | Topological sort views |
| `tests/views/test_search_base.cpp` | Search infrastructure |
| `tests/views/test_adaptors.cpp` | Pipe syntax |
| `tests/views/test_reverse_traversal.cpp` | Incoming edge views |
| `tests/views/test_view_chaining.cpp` | View composition |
| `tests/views/test_edge_data.cpp` | Edge data structs |
| `tests/views/test_neighbor_data.cpp` | Neighbor data structs |

---

## 7. Summary of All Changes by Type

### 7.1 Renames

| Old | New | Occurrences (est.) |
|-----|-----|--------------------|
| `vertex_info` | `vertex_data` | ~30 |
| `edge_info` | `edge_data` | ~40 |
| `neighbor_info` | `neighbor_data` | ~10 |
| `vertex_reference_t<G>` | `vertex_t<G>` | ~5 |
| `edge_reference_t<G>` | `edge_t<G>` | ~5 |
| `.source` member | `.source_id` member | ~5 |
| `.target` member | `.target_id` member | ~5 |
| `size(search)` | `search.num_visited()` | ~3 |
| `cancel(search, x)` | `search.cancel(x)` | ~3 |
| `depth(search)` | `search.depth()` | ~3 |

### 7.2 Additions

| Addition | Sections Affected |
|----------|------------------|
| `basic_` view variants (all 4 basic views) | vertexlist, incidence, neighbors, edgelist |
| `in_` view variants (incidence, neighbors) | incidence, neighbors |
| `_safe` topological sort factories | Topological sort |
| `transpose` view | New section |
| Pipe syntax / range adaptors | All sections |
| `search_view` concept | Common types |
| `vertex_value_function` / `edge_value_function` concepts | Introduction or referenced from GCI |
| Allocator parameter documentation | DFS, BFS, topological sort |
| Edge accessor exposition | Common types or exposition-only |
| Vertex ID in all basic view bindings | vertexlist, incidence, neighbors, edgelist |

### 7.3 Removals

| Removal | Sections Affected |
|---------|------------------|
| `sourced_edges_dfs` | DFS |
| `sourced_edges_bfs` | BFS |
| `sourced_edges_topological_sort` | Topological sort |
| Source/seed parameter from topological sort | Topological sort |
| All `\phil` notes | All sections |
| Free function `cancel()`, `depth()`, `size()` — replaced by members | Common types |

### 7.4 Structural Changes

| Change | Impact |
|--------|--------|
| `vertex_info<VorVId, VV>` (2 params) → `vertex_data<VId, V, VV>` (3 params) | All vertex views gain an extra binding field (uid) |
| `neighbor_info<VorVId, Sourced, VV>` (3 params) → `neighbor_data<VId, Sourced, V, VV>` (4 params) | Neighbor views gain both id and descriptor |
| Value functions: 1-arg → 2-arg | All value function examples and signatures change |
| Search views: vertex ID in binding → descriptor only (VId=void) | DFS, BFS, topological sort edge views lose target_id binding |
| Topological sort: seeded → all-vertex | Signature and usage examples change fundamentally |

---

## 8. Revision History Entry (for D3129r3)

Draft entry for `revision.tex`:

```latex
\subsection*{\paperno r3}
\begin{itemize}
    \item Renamed \tcode{vertex_info}, \tcode{edge_info}, and \tcode{neighbor_info} to
          \tcode{vertex_data}, \tcode{edge_data}, and \tcode{neighbor_data} respectively,
          with the addition of separate vertex ID and vertex descriptor template parameters.
    \item Updated all value functions from 1-arg \tcode{vvf(u)}/\tcode{evf(uv)} form to
          2-arg \tcode{vvf(g,u)}/\tcode{evf(g,uv)} form, taking the graph as the first
          parameter.
    \item Added \tcode{basic\_} view variants (\tcode{basic\_vertexlist},
          \tcode{basic\_incidence}, \tcode{basic\_neighbors}, \tcode{basic\_edgelist}) that
          yield only vertex/edge IDs without descriptors.
    \item Added incoming-edge view variants (\tcode{in\_incidence},
          \tcode{in\_neighbors}) for bidirectional graphs.
    \item Added \tcode{transpose} view for direction-swapping graph adaption.
    \item Added pipe syntax support via range adaptors for all view functions.
    \item Added \tcode{\_safe} topological sort factories with cycle detection
          via \tcode{std::expected}.
    \item Changed topological sort from seeded (single source) to all-vertex traversal.
    \item Removed \tcode{sourced\_edges\_dfs}, \tcode{sourced\_edges\_bfs}, and
          \tcode{sourced\_edges\_topological\_sort} — sourced search variants are no longer
          in the implementation.
    \item Changed search view control from free functions (\tcode{cancel()}, \tcode{depth()},
          \tcode{size()}) to member functions (\tcode{.cancel()}, \tcode{.depth()},
          \tcode{.num\_visited()}).
    \item Added \tcode{search\_view} concept.
    \item Replaced all uses of \tcode{vertex\_reference\_t<G>} with \tcode{vertex\_t<G>}
          and \tcode{edge\_reference\_t<G>} with \tcode{edge\_t<G>}, consistent with the
          descriptor-based architecture defined in D3130.
    \item Documented allocator parameters for search views (DFS, BFS, topological sort).
\end{itemize}
```
