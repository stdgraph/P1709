# GCI Document Update Strategy

## Purpose

This document captures all discrepancies between the **D3130 Container Interface** LaTeX
proposal and the **graph-v3** reference implementation in `graph-v3/include/graph/`, along
with a concrete plan to bring the LaTeX documents into alignment with the implementation.

**The implementation is the definitive/authoritative source.** Where the LaTeX and the
implementation disagree, the implementation is correct and the LaTeX must be updated.

### Relationship to `algo_update_strategy.md`

The algorithm strategy document (§0) describes the fundamental shift from reference-based
types (`vertex_reference_t<G>`, `edge_reference_t<G>`, `vertex_info`, `edge_info`) to
value-based descriptors (`vertex_t<G>`, `edge_t<G>`, `vertex_data`, `edge_data`).
`vertex_info` and `edge_info` were **renamed** to `vertex_data` and `edge_data` to
clarify that they represent the vertex and edge data models, respectively. That
shift **originates** in the GCI — it is the GCI that defines these types. This document
details the GCI-specific changes; the algorithm document consumes the types defined here.

Note: The algorithms have not yet been updated to work with vertices stored in mapped
(key-based) containers. The mapped container concepts and property maps are infrastructure
defined in the GCI for future algorithm support.

---

## 0. Fundamental Design Shift: Descriptor-Based Architecture

The most significant change in the GCI is the replacement of the old `descriptor` class
(a single monolithic class parameterized on `InnerIter`) with a **split descriptor
architecture** consisting of separate vertex and edge descriptor classes, each with a
corresponding descriptor view.

### 0.1 Old Design (LaTeX / D3130)

The LaTeX defines a **single** `descriptor<InnerIter>` class in
`D3130_Container_Interface/src/descriptor.hpp`:
- Parameterized on `forward_iterator InnerIter`
- Stores either an index (`ptrdiff_t`) or an iterator, depending on `random_access_iterator<InnerIter>`
- Full operator suite: `++`, `--`, `+=`, `-=`, `+`, `-`, `[]`, `==`, `<=>`
- `inner_value()` member to access the underlying container element
- `descriptor_view(R&&)` and `descriptor_subrange_view(R&&, R&&)` factory functions

### 0.2 New Design (Implementation)

The implementation replaces the single class with **four distinct types** in the
`graph::adj_list` namespace:

| New Type (Implementation)                                    | Purpose                                  | File                         |
| ------------------------------------------------------------ | ---------------------------------------- | ---------------------------- |
| `vertex_descriptor<VertexIter>`                              | Lightweight vertex handle                | `vertex_descriptor.hpp`      |
| `edge_descriptor<EdgeIter, VertexIter, EdgeDirection>`       | Lightweight edge handle with source info | `edge_descriptor.hpp`        |
| `vertex_descriptor_view<VertexIter>`                         | View over vertex container               | `vertex_descriptor_view.hpp` |
| `edge_descriptor_view<EdgeIter, VertexIter, EdgeDirection>`  | View over edge container for one vertex  | `edge_descriptor_view.hpp`   |

Key design differences:
- **`vertex_descriptor`** uses `storage_type = conditional_t<random_access_iterator, size_t, VertexIter>`
  (note: `size_t`, not `ptrdiff_t`)
- **`edge_descriptor`** stores both the edge position (`edge_storage_type`) and the source
  vertex (`vertex_desc`), enabling `source_id()` without external lookup
- **`EdgeDirection`** tag class (`out_edge_tag` / `in_edge_tag`) distinguishes outgoing from
  incoming edge descriptors, enabling bidirectional graph support
- Inner value access is via `underlying_value(container)` and `inner_value(container)`, not
  `inner_value()` — the container must be passed in
- `vertex_id()` is a member of `vertex_descriptor` (not just a free function)

### 0.2a Why Descriptors? (Must Include in Paper)

The graph-v3 documentation (`docs/user-guide/adjacency-lists.md`, §6) contains an excellent
motivation for the descriptor design that **must** be captured in the D3130 paper. The paper
already has a prose paragraph on descriptors (§"Vertex and Edge Descriptor Views") but it
lacks the crisp comparison table and the key insight about implicit vertices.

**Key points to incorporate into D3130:**

1. **Abstraction** — A single descriptor type can represent both an index into a `vector` and
   an iterator into a `map`. This means the library needs far fewer concept definitions and
   function overloads than an iterator-centric or reference-centric design.

2. **Implicit vertex support** — Index-based vertex descriptors can refer to a vertex even
   when no physical vertex object exists (e.g., `compressed_graph` or an implicit grid). The
   vertex is identified purely by its index, so algorithms and views work the same way
   regardless of whether the container materializes vertex storage.

3. **Comparison table** — The following table should appear in the paper's descriptor section:

   | Concern            | Raw iterators                                   | Descriptors                                                                    |
   | ------------------ | ----------------------------------------------- | ------------------------------------------------------------------------------ |
   | Abstraction        | Expose container layout                         | Hide storage strategy — one concept covers indexed and keyed graphs            |
   | Overload reduction | Separate overloads for index vs. iterator        | A single overload accepts the descriptor                                       |
   | Invalidation       | Invalidated by container mutation                | Can be re-resolved via `find_vertex`                                           |
   | Copyability        | Varies (e.g., `forward_list::iterator`)          | Always trivially copyable or cheap                                             |
   | Size               | May be pointer-sized or larger                   | Index-based: one integer                                                       |

4. **Automatic pattern recognition** — CPOs detect containers automatically:
   - **Random-access** containers (`vector`, `deque`) → index-based IDs
   - **Associative** containers (`map`, `unordered_map`) → key-based IDs
   - **Edge patterns** → `int`, `pair<int, W>`, `tuple<int, ...>`, or structs are recognized
     to automatically extract edge properties (target ID, edge value, etc.)

**Source:** `graph-v3/docs/user-guide/adjacency-lists.md`, §6 "Descriptors" (lines 276–345).

**Action:** When rewriting the "Vertex and Edge Descriptor Views" section of
`container_interface.tex` (Phase 1, task 4), incorporate this motivation material prominently.
The comparison table and implicit-vertex insight belong near the beginning of the descriptor
discussion, before the type definitions.

### 0.3 Descriptor Traits (New)

The implementation adds a comprehensive trait system not in the LaTeX:

| Trait / Concept                              | Purpose                                          |
| -------------------------------------------- | ------------------------------------------------ |
| `is_vertex_descriptor<T>` / `_v`             | Detect vertex descriptors                        |
| `is_edge_descriptor<T>` / `_v`               | Detect edge descriptors                          |
| `is_in_edge_descriptor<T>` / `_v`            | Detect incoming edge descriptors                 |
| `is_descriptor<T>` / `_v`                    | Either vertex or edge descriptor                 |
| `is_vertex_descriptor_view<T>` / `_v`        | Detect vertex descriptor views                   |
| `is_edge_descriptor_view<T>` / `_v`          | Detect edge descriptor views                     |
| `vertex_descriptor_type<T>` (concept)        | Constrained concept for vertex descriptors       |
| `edge_descriptor_type<T>` (concept)          | Constrained concept for edge descriptors         |
| `descriptor_type<T>` (concept)               | Either descriptor type                           |
| `random_access_descriptor<T>` (concept)      | Descriptor with index-based storage              |
| `iterator_based_descriptor<T>` (concept)     | Descriptor with iterator-based storage           |

### 0.4 Descriptor Pattern Detection (New)

The implementation defines concepts for detecting edge and vertex patterns:

**Vertex patterns:**
- `direct_vertex_type<Iter>` — random-access (ID = index)
- `keyed_vertex_type<Iter>` — bidirectional + pair-like (ID = key, for maps)
- `random_access_vertex_pattern<Iter>` — returns whole container element
- `pair_value_vertex_pattern<Iter>` — returns `.second` (for maps)
- `whole_value_vertex_pattern<Iter>` — returns whole dereferenced value

**Edge patterns:**
- `simple_edge_type<T>` — integral type (just target ID)
- `pair_edge_type<T>` — pair with `.first`=target, `.second`=properties
- `tuple_edge_type<T>` — tuple with first element = target
- `custom_edge_type<T>` — custom struct (user manages)
- `edge_value_type<T>` — union of all above

**Action:** Rewrite `D3130_Container_Interface/src/descriptor.hpp` and
`D3130_Container_Interface/src/descriptor_view.hpp` to reflect the split descriptor
architecture. Add descriptor traits and pattern detection concepts to the proposal.

---

## 1. Concept Discrepancies

### 1.1 Edge Concepts — `edge_reference_t<G>` → `edge_t<G>`

| Aspect               | LaTeX (D3130)                                        | Implementation                                                        |
| --------------------- | ---------------------------------------------------- | --------------------------------------------------------------------- |
| Edge parameter type   | `edge_reference_t<G> uv`                             | `const E& e` where `E` satisfies `edge_descriptor_type`              |
| `targeted_edge`       | `requires(G&& g, edge_reference_t<G> uv)`            | Subsumed by `edge<G, E>` concept on edge descriptors                 |
| `sourced_edge`        | `requires(G&& g, edge_reference_t<G> uv)`            | Subsumed by `edge<G, E>` concept (all descriptors have source info)  |
| `sourced_targeted_edge` | Compound concept                                   | Replaced by single `edge<G, E>` concept                              |
| Unified concept       | 3 separate edge concepts + 3 basic variants           | Single `edge<G, E>` concept requiring all operations                 |

**Implementation's `edge<G, E>` concept** (from `adjacency_list_concepts.hpp`):
```cpp
template <class G, class E>
concept edge = is_edge_descriptor_v<remove_cvref_t<E>> && requires(G& g, const E& e) {
    target_id(g, e);   target(g, e);
    source_id(g, e);   source(g, e);
};
```

The `basic_*` variants (`basic_targeted_edge`, `basic_sourced_edge`, etc.) in
`concepts_edges_before.hpp` no longer exist in the implementation.

**Action:**
1. Replace all `targeted_edge`, `sourced_edge`, `sourced_targeted_edge`, `basic_targeted_edge`,
   `basic_sourced_edge`, `basic_sourced_targeted_edge` concepts with the single `edge<G, E>`.
2. Remove `concepts_edges.hpp` and `concepts_edges_before.hpp` source files.
3. Add the `edge<G, E>` concept to the proposal.

### 1.2 Edge Range Concepts

| Aspect                     | LaTeX (D3130)                        | Implementation                                          |
| -------------------------- | ------------------------------------ | ------------------------------------------------------- |
| `targeted_edge_range`      | Uses `basic_targeted_edge_range<G>`  | Replaced by `out_edge_range<R, G>`                      |
| `basic_targeted_edge_range`| Intermediate concept                  | Does not exist                                          |
| Out-edge range             | `targeted_edge_range<G>`             | `out_edge_range<R, G>`: forward_range + edge<G, value_type> |
| In-edge range              | Not in LaTeX                         | `in_edge_range<R, G>`: forward_range + edge<G, value_type> |

**Implementation:**
```cpp
template <class R, class G>
concept out_edge_range = forward_range<R> && edge<G, range_value_t<R>>;

template <class R, class G>
concept in_edge_range = forward_range<R> && edge<G, range_value_t<R>>;
```

**Action:** Replace `targeted_edge_range` and `basic_targeted_edge_range` with
`out_edge_range<R, G>`. Add `in_edge_range<R, G>`. Remove `concepts_target_edge_range.hpp`.

### 1.3 Vertex Concepts

| Aspect                | LaTeX (D3130)                                     | Implementation                                            |
| --------------------- | ------------------------------------------------- | --------------------------------------------------------- |
| `_common_vertex_range`| Uses `vertex_iterator_t<G>`                        | Not present; replaced by `vertex<G, V>` concept           |
| `vertex_range`        | `forward_range<vertex_range_t<G>>`                 | `forward_range<R> && sized_range<R> && vertex<G, value_t<R>>` |
| `index_vertex_range`  | `random_access_range<vertex_range_t<G>> && integral<vertex_id_t<G>>` | Checks `integral<vertex_id_t<G>>` AND `integral<vertex_range_t<G>::storage_type>` |
| `vertex<G, V>`        | Not in LaTeX                                       | New concept: `is_vertex_descriptor_v<V>` + vertex_id/find_vertex |
| `mapped_vertex_range`  | Not in LaTeX                                      | New: `!index_vertex_range<G> && hashable_vertex_id<G>`    |

**Implementation's `vertex<G, V>` concept:**
```cpp
template <class G, class V>
concept vertex = is_vertex_descriptor_v<remove_cvref_t<V>> &&
                 requires(G& g, const V& u) {
                     vertex_id(g, u);
                     find_vertex(g, vertex_id(g, u));
                 };
```

**Action:**
1. Replace `_common_vertex_range` with the `vertex<G, V>` concept.
2. Update `vertex_range` and `index_vertex_range` definitions.
3. Add `mapped_vertex_range` concept.
4. Remove `concepts_vertex_range.hpp`, replace with new source file.

### 1.4 Adjacency List Concepts

| Aspect                                 | LaTeX (D3130)                          | Implementation                              |
| -------------------------------------- | -------------------------------------- | ------------------------------------------- |
| `adjacency_list`                       | `vertex_range<G> && targeted_edge_range<G> && targeted_edge<G>` | `requires { vertices(g) } -> vertex_range<G>; { out_edges(g,u) } -> out_edge_range<G>` |
| `index_adjacency_list`                 | `index_vertex_range<G> && targeted_edge_range<G> && targeted_edge<G>` | `adjacency_list<G> && index_vertex_range<G>` |
| `sourced_adjacency_list`               | Present                                | **Removed** — source info is always available via descriptors |
| `sourced_index_adjacency_list`         | Present                                | **Removed**                                 |
| `basic_adjacency_list`                 | Present (in concepts_basic_adj_list.hpp) | **Removed**                              |
| `basic_index_adjacency_list`           | Present                                | **Removed**                                 |
| `basic_sourced_adjacency_list`         | Present                                | **Removed**                                 |
| `basic_sourced_index_adjacency_list`   | Present                                | **Removed**                                 |
| `ordered_vertex_edges`                 | Not in LaTeX                           | New concept for sorted adjacency lists      |
| `bidirectional_adjacency_list`         | Not in LaTeX                           | New: adjacency_list + in_edges support      |
| `index_bidirectional_adjacency_list`   | Not in LaTeX                           | New: bidirectional + index vertices         |
| `hashable_vertex_id`                   | Not in LaTeX                           | New: vertex IDs usable as unordered_map keys |
| `mapped_adjacency_list`                | Not in LaTeX                           | New: adjacency_list + mapped_vertex_range   |
| `mapped_bidirectional_adjacency_list`  | Not in LaTeX                           | New: bidirectional + mapped vertices        |

**Action:**
1. Remove all `basic_*` and `sourced_*` adjacency list concepts — the descriptor design makes
   "sourced" vs. "unsourced" distinction unnecessary (all edge descriptors carry source info
   via the source vertex_descriptor stored in the edge_descriptor).
2. Remove `concepts_basic_adj_list.hpp` source file.
3. Add `ordered_vertex_edges`, `bidirectional_adjacency_list`,
   `index_bidirectional_adjacency_list`, `hashable_vertex_id`, `mapped_vertex_range`,
   `mapped_adjacency_list`, `mapped_bidirectional_adjacency_list` concepts.
4. Rewrite `concepts_adj_list.hpp` to match the implementation.

---

## 2. Type Alias Discrepancies

### 2.1 Type Aliases Table

The LaTeX Type Aliases table (Table 2) needs significant updates:

| Type Alias              | LaTeX (D3130)                                  | Implementation                                           | Change |
| ----------------------- | ---------------------------------------------- | -------------------------------------------------------- | ------ |
| `vertex_range_t<G>`     | `decltype(vertices(g))`                        | Same                                                     | OK     |
| `vertex_iterator_t<G>`  | `iterator_t<vertex_range_t<G>>`                | Same                                                     | OK     |
| `vertex_t<G>`           | `range_value_t<vertex_range_t<G>>`             | Same, but now a `vertex_descriptor<Iter>` type — **replaces `vertex_reference_t<G>`** | Clarify |
| `vertex_reference_t<G>` | `range_reference_t<vertex_range_t<G>>`         | **Removed** — replaced by `vertex_t<G>` (a descriptor)   | Remove |
| `vertex_id_t<G>`        | `decltype(vertex_id(g,u))`                     | `remove_cvref_t<decltype(vertex_id(g, vertex_t<G>))>`    | Update |
| `vertex_value_t<G>`     | `decltype(vertex_value(g,u))`                  | Same (via CPO)                                           | OK     |
| `vertex_edge_range_t<G>`| `decltype(edges(g,u))`                         | `out_edge_range_t<G>` (alias)                            | Update |
| `vertex_edge_iterator_t<G>` | `iterator_t<vertex_edge_range_t<G>>`       | `out_edge_iterator_t<G>` (alias)                         | Update |
| `edge_t<G>`             | `range_value_t<vertex_edge_range_t<G>>`        | `out_edge_t<G>` (alias); now `edge_descriptor<...>` type — **replaces `edge_reference_t<G>`** | Clarify |
| `edge_reference_t<G>`   | `range_reference_t<vertex_edge_range_t<G>>`    | **Removed** — replaced by `edge_t<G>` (a descriptor)     | Remove |
| `edge_value_t<G>`       | `decltype(edge_value(g,uv))`                   | Same (via CPO)                                           | OK     |
| `graph_reference_t<G>`  | `add_lvalue_reference<G>`                      | Not in implementation                                    | Review |
| `graph_value_t<G>`      | `decltype(graph_value(g))`                     | Same (via CPO)                                           | OK     |
| (new) `raw_vertex_id_t<G>` | Not in LaTeX                                | `decltype(vertex_id(g, vertex_t<G>))` — preserves references | Add    |
| (new) `out_edge_range_t<G>` | Not in LaTeX                               | `decltype(out_edges(g, vertex_t<G>))`                    | Add    |
| (new) `out_edge_iterator_t<G>` | Not in LaTeX                            | `iterator_t<out_edge_range_t<G>>`                        | Add    |
| (new) `out_edge_t<G>`   | Not in LaTeX                                   | `range_value_t<out_edge_range_t<G>>`                     | Add    |
| (new) `in_edge_range_t<G>` | Not in LaTeX                                | `decltype(in_edges(g, vertex_t<G>))`                     | Add    |
| (new) `in_edge_iterator_t<G>` | Not in LaTeX                             | `iterator_t<in_edge_range_t<G>>`                         | Add    |
| (new) `in_edge_t<G>`    | Not in LaTeX                                   | `range_value_t<in_edge_range_t<G>>`                      | Add    |

**Action:** Remove `vertex_reference_t<G>`, `edge_reference_t<G>`, and `graph_reference_t<G>`.
Every use of `vertex_reference_t<G>` is replaced by `vertex_t<G>` and every use of
`edge_reference_t<G>` is replaced by `edge_t<G>` — both are now lightweight descriptor
types (value types, not references). Add the `out_edge_*`, `in_edge_*`, and
`raw_vertex_id_t<G>` aliases. Document that `vertex_edge_range_t`, `vertex_edge_iterator_t`,
and `edge_t` are backward-compatibility aliases for `out_edge_range_t`,
`out_edge_iterator_t`, and `out_edge_t`.

---

## 3. Traits Table Discrepancies

### 3.1 Traits Table

| Trait (LaTeX)                               | Implementation Status                       | Change |
| ------------------------------------------- | ------------------------------------------- | ------ |
| `has_degree<G>`                             | Present (concept + `_v`)                    | OK     |
| `has_find_vertex<G>`                        | Present (concept + `_v`)                    | OK     |
| `has_find_vertex_edge<G>`                   | Present (concept + `_v`)                    | OK     |
| `has_contains_edge<G>`                      | Present (concept + `_v`) — now `has_contains_edge<G, V>` | Update (added V param) |
| `define_unordered_edge<G>`                  | **Removed** — not in implementation         | Remove |
| `unordered_edge<G>`                         | **Removed** — not in implementation         | Remove |
| `ordered_edge<G>`                           | **Removed** — not in implementation         | Remove |
| `define_adjacency_matrix<G>`                | **Removed** — not in implementation         | Remove |
| `is_adjacency_matrix<G>` / `_v`            | **Removed** — not in implementation         | Remove |
| `adjacency_matrix<G>`                       | **Removed** — not in implementation         | Remove |
| (new) `has_in_degree<G>` / `_v`            | New in implementation                       | Add    |
| (new) `has_find_in_edge<G>` / `_v`         | New in implementation                       | Add    |
| (new) `has_contains_in_edge<G, V>` / `_v`  | New in implementation                       | Add    |
| (new) `has_basic_queries<G>` / `_v`        | New: degree+find_vertex+find_vertex_edge    | Add    |
| (new) `has_full_queries<G>` / `_v`         | New: basic_queries+contains_edge            | Add    |

**Action:** Remove adjacency_matrix and unordered/ordered edge traits. Add all bidirectional
query traits and combined query traits. Update `has_contains_edge` to take a second template
parameter `V`.

---

## 4. Function / CPO Discrepancies

### 4.1 CPO Naming — `edges` → `out_edges`

| LaTeX Name               | Implementation Name    | Notes                                       |
| ------------------------ | ---------------------- | ------------------------------------------- |
| `edges(g, u)`            | `out_edges(g, u)`      | `edges` kept as alias: `auto& edges = out_edges` |
| `edges(g, uid)`          | `out_edges(g, uid)`    | Same alias pattern                          |
| `degree(g, u)`           | `out_degree(g, u)`     | `degree` kept as alias: `auto& degree = out_degree` |
| `find_vertex_edge(g,...)`| `find_out_edge(g,...)`  | `find_vertex_edge` kept as alias            |
| `contains_edge(g,...)`   | `contains_out_edge(g,...)`| `contains_edge` kept as alias             |
| `has_edge(g)`            | `has_edges(g)`         | Renamed (plural)                            |

**Action:** Update the function tables to use the new primary names (`out_edges`, `out_degree`,
`find_out_edge`, `contains_out_edge`, `has_edges`). Note that old names are retained as
backward-compatible aliases.

### 4.2 New CPOs (Not in LaTeX)

| CPO                                      | Signature(s)                                                | Purpose                     |
| ---------------------------------------- | ----------------------------------------------------------- | --------------------------- |
| `in_edges(g, u)` / `in_edges(g, uid)`  | Returns `in_edge_range_t<G>`                                 | Incoming edges              |
| `in_degree(g, u)` / `in_degree(g, uid)` | Returns integral                                            | Incoming edge count         |
| `source(g, uv)`                          | Returns `vertex_t<G>` (via source_id + find_vertex default) | Source vertex descriptor    |
| `find_in_edge(g, u, v)` (3 overloads)   | Returns in-edge iterator                                    | Find incoming edge          |
| `contains_in_edge(g, u, v)` (2 overloads)| Returns bool                                               | Test for incoming edge      |

**Action:** Add these CPOs to the edge function and vertex function tables.

### 4.3 CPO Resolution Order

The LaTeX describes functions with "Default Implementation" but doesn't specify the full
CPO resolution order. The implementation uses a 3–4 tier resolution for each CPO:

1. **Member function** on the graph: `g.fn(args)`
2. **ADL**: `fn(g, args)` found via ADL
3. **Descriptor member**: method on the descriptor itself (where applicable)
4. **Default implementation**: computed from other CPOs

**Action:** Decide whether to document the full CPO resolution hierarchy or keep the current
approach of showing only the default implementation. At minimum, mention that member and ADL
overrides take priority.

### 4.4 Edge Function Parameter Changes

| Aspect                        | LaTeX (D3130)                  | Implementation                    |
| ----------------------------- | ------------------------------ | --------------------------------- |
| `target_id` parameter type    | `edge_reference_t<G> uv`      | Shared CPO: works on `edge_t<G>` (out-edge descriptor), `in_edge_t<G>`, `edge_list::edge_descriptor`, `edge_data`, or tuple-like types |
| `source_id` parameter type    | `edge_reference_t<G> uv`      | Same as target_id — multi-tier resolution |
| `edge_value` parameter type   | `edge_reference_t<G> uv`      | Same — now `edge_value(g, uv)` (2 args, shared CPO) |
| `target` return type          | `vertex_t<G>`                  | Same                               |
| `source` return type          | `vertex_t<G>`                  | Same (new CPO)                     |

Note: `target_id`, `source_id`, and `edge_value` are now **shared CPOs** defined in
`graph/detail/edge_cpo.hpp`, used by both adjacency list and edge list interfaces. They use
a multi-tier resolution strategy:
1. Native edge member (highest priority)
2. ADL with descriptor
3. `adj_list::edge_descriptor` member
4. `edge_list::edge_descriptor` member
5. `edge_data` data member access
6. Tuple/pair access (lowest priority)

**Action:** Update the edge function table to reflect the unified CPO approach. Replace
`edge_reference_t<G>` parameters with `edge_t<G>` (or appropriate descriptor type).

### 4.5 Edgelist Function Arity

| Function           | LaTeX (D3130)          | Implementation                 |
| ------------------ | ---------------------- | ------------------------------ |
| `source_id(e)`     | 1-arg                  | `source_id(el, uv)` (2-arg)   |
| `target_id(e)`     | 1-arg                  | `target_id(el, uv)` (2-arg)   |
| `edge_value(e)`    | 1-arg                  | `edge_value(el, uv)` (2-arg)  |
| `contains_edge(el,uid,vid)` | 3-arg         | Same                           |
| `num_edges(el)`    | 1-arg                  | Same                           |
| `has_edge(el)`     | 1-arg                  | Same                           |

The edgelist CPOs now take the edgelist container as the first argument (matching the
adjacency list convention). This is a significant API change.

**Action:** Update edgelist function table and source files to use 2-arg form for
`source_id`, `target_id`, and `edge_value`.

---

## 5. Edgelist Concept Discrepancies

| Concept                     | LaTeX (D3130)                                    | Implementation                                              |
| --------------------------- | ------------------------------------------------ | ----------------------------------------------------------- |
| `sourced_edgelist`          | `requires(range_value_t<EL> e) { source_id(e); target_id(e); }` | `basic_sourced_edgelist`: `requires(EL& el, range_value_t<EL> uv) { source_id(el, uv); target_id(el, uv); }` |
| `sourced_index_edgelist`    | `{ source_id(e) } -> integral`                   | `basic_sourced_index_edgelist`: integral check on 2-arg form |
| `has_edge_value`            | `{ edge_value(e) }`                               | `requires(EL& el, range_value_t<EL> uv) { edge_value(el, uv); }` |
| Return type checking        | `same_as<decltype(source_id(e))>` on target_id   | `convertible_to<decltype(source_id(...))>` on target_id      |

**Action:** Update edgelist concepts to use 2-arg CPO form. Rename `sourced_edgelist` →
`basic_sourced_edgelist`. Change `same_as` to `convertible_to` for target_id check.

---

## 6. Edgelist Type Alias Discrepancies

| Type Alias (LaTeX)         | Implementation Change                                                    |
| -------------------------- | ------------------------------------------------------------------------ |
| `edge_value_t<EL>`        | Now constrained by `has_edge_value` concept, not `basic_sourced_edgelist` |
| `vertex_id_t<EL>`         | Uses `remove_cvref_t<>` on result of 2-arg `source_id(el, uv)`          |
| (new) `raw_vertex_id_t<EL>` | Preserves reference-ness of source_id return                           |
| `edge_reference_t<EL>`    | Still present in edgelist types — review if needed                       |

**Action:** Update edgelist type aliases. Add `raw_vertex_id_t<EL>`. Review whether
`edge_reference_t<EL>` should be removed from the edgelist as well.

---

## 7. Edgelist Patterns — `edge_info` renamed to `edge_data`

`edge_info` was **renamed** to `edge_data` (and `vertex_info` to `vertex_data`) to
clarify that these types represent the edge and vertex data models, respectively.

| Pattern (LaTeX)                              | Pattern (Implementation)                    |
| -------------------------------------------- | ------------------------------------------- |
| `edge_info<integral, true, void, void>`      | `edge_data<VId, true, void, void>`          |
| `edge_info<integral, true, void, scalar>`    | `edge_data<VId, true, void, EV>`            |
| `tuple<integral, integral>`                  | Same                                        |
| `tuple<integral, integral, scalar>`          | Same                                        |
| (new) `pair<T, T>`                           | Also supported                              |
| (new) `edge_data<VId, true, E&, void>`       | With edge descriptor reference              |
| (new) `edge_data<VId, true, E&, EV>`         | With edge descriptor reference and value    |

**Action:** Rename all `edge_info` → `edge_data` and `vertex_info` → `vertex_data`
throughout the paper (these are renames, not removals — the types still exist under their
new names reflecting their role as data models). Document the full set of supported edge
patterns including `pair` and edge descriptor reference variants.

---

## 8. Data Structures — `vertex_data` and `edge_data` (New)

These types are defined in `graph_data.hpp` and used by views for structured bindings.
They are **not in the current LaTeX**.

### 8.1 `vertex_data<VId, V, VV>`

Template struct with 8 specializations (void combinations of VId, V, VV):
- `{id, vertex, value}` — full
- `{id, vertex}` — no value
- `{id, value}` — no vertex descriptor
- `{id}` — id only
- `{vertex, value}` — no id (descriptor-based)
- `{vertex}` — descriptor only
- `{value}` — value only
- `{}` — empty

### 8.2 `edge_data<VId, Sourced, E, EV>`

Template struct with multiple specializations for sourced/unsourced × void combinations:
- Full: `{source_id, target_id, edge, value}` (sourced)
- `{target_id, edge, value}` (unsourced)
- Various void specializations

### 8.3 `neighbor_data<VId, Sourced, V, VV>`

Similar to `edge_data` but for neighbor views.

### 8.4 Helper Aliases

| Alias                    | Definition                                    |
| ------------------------ | --------------------------------------------- |
| `copyable_vertex_t<VId, VV>` | `vertex_data<VId, void, VV>` — `{id, value}` |
| `copyable_edge_t<VId, EV>`   | `edge_data<VId, true, void, EV>` — `{source_id, target_id, value}` |
| `copyable_neighbor_t<VId, VV>` | `neighbor_data<VId, true, void, VV>`        |

### 8.5 `graph_error` Exception

| Aspect     | LaTeX                                  | Implementation                            |
| ---------- | -------------------------------------- | ----------------------------------------- |
| Base class | `runtime_error`                        | `runtime_error`                           |
| Constructors | Not specified                        | `explicit(const string&)`, `explicit(const char*)` |

**Action:** Add `vertex_data`, `edge_data`, `neighbor_data`, helper aliases, and
`graph_error` to the proposal. These are fundamental to the view iteration model.

---

## 9. Vertex Property Map (New)

The implementation adds `vertex_property_map` infrastructure in
`adj_list/vertex_property_map.hpp` — not in the LaTeX at all.

| Component                                     | Purpose                                         |
| --------------------------------------------- | ----------------------------------------------- |
| `vertex_property_map<G, T>`                   | Type alias: `vector<T>` for index graphs, `unordered_map<VId, T>` for mapped |
| `make_vertex_property_map(g, init_value)`      | Eager factory (all vertices pre-populated)      |
| `make_vertex_property_map<G, T>(g)`            | Lazy factory (capacity-reserved)                |
| `vertex_property_map_contains(map, uid)`       | Test for key presence                           |
| `vertex_property_map_get(map, uid, default_val)` | Read with default fallback                    |

This is critical infrastructure for algorithms on mapped (key-based) graphs, where algorithms
cannot simply index into a `vector` with a vertex ID.

**Action:** Decide whether to include `vertex_property_map` in this proposal. It is
infrastructure needed for mapped graph algorithm support. If included, add a new subsection.

---

## 10. Value Function Concepts (New)

The implementation defines `vertex_value_function` and `edge_value_function` concepts in
`graph_concepts.hpp` — not in the LaTeX:

```cpp
template <class VVF, class Graph, class VertexDescriptor>
concept vertex_value_function = invocable<VVF, const Graph&, VertexDescriptor> &&
                                (!is_void_v<invoke_result_t<VVF, const Graph&, VertexDescriptor>>);

template <class EVF, class Graph, class EdgeDescriptor>
concept edge_value_function = invocable<EVF, const Graph&, EdgeDescriptor> &&
                              (!is_void_v<invoke_result_t<EVF, const Graph&, EdgeDescriptor>>);
```

These are the base concepts that `basic_edge_weight_function` (in algorithms) refines.
They take `(const Graph&, Descriptor)` — matching the 2-arg convention.

**Action:** Add `vertex_value_function` and `edge_value_function` to the GCI proposal. These
are used by views (vertexlist, incidence, etc.) and algorithms alike.

---

## 11. Namespace Organization

| Aspect              | LaTeX (D3130)                  | Implementation                                      |
| ------------------- | ------------------------------ | --------------------------------------------------- |
| Adj list namespace  | `std::graph`                   | `graph::adj_list` (CPOs imported into `graph::`)    |
| Edgelist namespace  | `std::graph::edgelist`         | `graph::edge_list`                                  |
| Views namespace     | Not specified in D3130         | `graph::views`                                      |
| Container namespace | Not specified                  | `graph::container`                                  |
| Detail namespace    | Not specified                  | `graph::detail` (shared), `graph::adj_list::detail` |

**Action:** Update namespace references. Note that `adj_list` CPOs are imported into the
`graph::` namespace via `using` declarations for backward compatibility / convenience.

---

## 12. Multipartite / Partition Support

The partition support in the LaTeX and implementation are largely aligned:

| Aspect                  | LaTeX (D3130)                    | Implementation                                    |
| ----------------------- | -------------------------------- | ------------------------------------------------- |
| `num_partitions(g)`     | Returns integral, default 1      | Same                                              |
| `vertices(g, pid)`      | Returns partition vertex range   | Same                                              |
| `num_vertices(g, pid)`  | Returns integral                 | Same                                              |
| `partition_id(g, u)`    | Returns `partition_id_t<G>`      | Same, with descriptor-based default returning 0   |

No major changes needed here.

---

## 13. Determining vertex_id Type

| Step | LaTeX (D3130)                                                     | Implementation                                        |
| ---- | ----------------------------------------------------------------- | ----------------------------------------------------- |
| 1    | Use vertex_id(g,u) override                                       | Same (member → ADL → default)                        |
| 2    | Pattern: `random_access_range<forward_range<integral>>`           | Subsumed by descriptor pattern detection              |
| 3    | Default: `size_t`                                                  | Default: `size_t` (for random-access); key type for maps |

The implementation determines vertex_id type through the descriptor:
- For `random_access_iterator<VertexIter>`: `storage_type = size_t`
- For `keyed_vertex_type<VertexIter>` (maps): key type from the pair-like iterator

**Action:** Update the "Determining the vertex_id and its type" section to reflect
the descriptor-based approach. The pattern-matching description is obsolete.

---

## 14. Missing Information / Open Questions

### Q1 — Descriptor Exposition Level
**Question:** How much internal descriptor implementation detail should the proposal expose?
The current implementation has rich internals (storage_type, underlying_value, inner_value,
pattern detection concepts). The proposal needs to define the *observable interface* without
over-specifying implementation details.

**Options:**
- (a) Show full descriptor class specifications (as-is from implementation)
- (b) Show only the interface users interact with (vertex_id, inner_value, comparison,
  increment) and mark internals as exposition-only
- (c) Define descriptors purely through their concepts and observable behavior

**Decision: Hybrid (b)+(c), following WG21 convention.**

Standards proposals specify *what*, not *how*. The approach mirrors how the standard handles
`std::optional` (exposition-only `*val*`), `std::expected`, and range adaptors:

1. **Concepts are normative.** `vertex_descriptor_type`, `edge_descriptor_type`,
   `random_access_descriptor`, `iterator_based_descriptor` — fully specified as the primary
   contract. This is how `iterator`, `range`, `allocator` are specified in the standard.

2. **Descriptor classes are the default implementation.** `vertex_descriptor<Iter>` and
   `edge_descriptor<...>` are specified with exposition-only data members (italic names per
   WG21 convention: *`storage-type`*, *`underlying-value`*) and a public observable interface
   (construction, `vertex_id()`, comparison, increment). Implementations may use different
   internal representations as long as observable behavior matches.

3. **Pattern detection concepts are fully specified.** `simple_edge_type`, `pair_edge_type`,
   `tuple_edge_type`, etc. define how the library auto-detects user data structures — these
   are part of the customization-point machinery and user code depends on them.

4. **Descriptor traits are fully specified.** `is_vertex_descriptor_v`,
   `is_edge_descriptor_v`, etc. are used in SFINAE/concepts by user code and must be normative.

### Q2 — Descriptor Passing Convention and `vertex_reference_t` / `edge_reference_t` Replacement

**Question:** The paper text (not just the code) uses `vertex_reference_t<G>` and
`edge_reference_t<G>` in explanatory prose and concept definitions. All such references
must be replaced with `vertex_t<G>` and `edge_t<G>` respectively — the descriptor types
that supersede them. Should descriptors be passed by value or by `const&`?

**Decision: Pass descriptors by value in function/CPO signatures; use `const&` only in
concept `requires` clauses.**

Size analysis of the implementation's descriptors:

| Descriptor | Members | Size (all configurations) | Trivially copyable |
|---|---|---|---|
| `vertex_descriptor` | 1 field (`size_t` or iterator) | **8 bytes** | Yes |
| `edge_descriptor` | 2 fields (edge position + source vertex) | **16 bytes** | Yes |

`EdgeDirection` is a stateless tag — zero instance overhead. Even for iterator-based
containers (maps, lists), standard library iterators are a single pointer (8 bytes), so
`edge_descriptor` is always exactly 16 bytes.

**Rationale for pass-by-value:**

1. **16 bytes is the pass-by-value sweet spot.** `string_view` and `span` (both 16 bytes)
   are passed by value in the standard. Edge descriptors are the same size.
2. **Register-passed on all major ABIs.** Both descriptors fit in registers:

   | ABI             | Register-passed limit | vertex (8B) | edge (16B)                  |
   |-----------------|----------------------|-------------|-----------------------------|
   | System V x86-64 | 16 bytes in 2 regs   | 1 register  | 2 registers                 |
   | ARM64 (AAPCS64) | 16 bytes in 2 regs   | 1 register  | 2 registers                 |
   | MSVC x64        | 8 bytes in 1 reg     | 1 register  | Indirect (pointer), but optimized away for inlined CPOs |

   ARM64 has a hard 16-byte cutoff — at 17 bytes the ABI switches to indirect passing.
   Our descriptors sit exactly at or below the limit. MSVC x64 technically passes >8-byte
   types by pointer, but graph CPOs are `constexpr` function objects that inline, so the
   optimizer eliminates the indirection. `string_view` (16 bytes) uses the same by-value
   convention on MSVC.
3. **No aliasing concerns.** Pass-by-value eliminates the possibility of the descriptor
   aliasing the graph's internal storage — the compiler can optimize more aggressively.
4. **Matches the iterator convention.** The standard passes iterators by value universally
   (`std::sort(It first, It last)`). Descriptors are the moral equivalent of iterators.

**Convention distinction:**
- **Concept requirements:** `requires(G& g, const E& e) { target_id(g, e); }` — uses
  `const&` (standard practice for expressing requirements on types)
- **Function/CPO signatures in the paper:** `target_id(G& g, edge_t<G> e)` — pass by value
- **User code:** Pass by value — `auto eid = target_id(g, e);`

There is no case where `const&` is needed for the standard-provided descriptors. The
descriptor concepts should imply efficient copyability (like iterator requirements).

**Action needed:** Full text search of `container_interface.tex` for `vertex_reference`,
`edge_reference`, `vertex_info`, `edge_info` in prose text. Replace every occurrence of
`vertex_reference_t<G>` with `vertex_t<G>`, `edge_reference_t<G>` with `edge_t<G>`,
`vertex_info` with `vertex_data`, and `edge_info` with `edge_data` (the latter two are
renames reflecting the data model role). Ensure all function signatures pass descriptors
by value, not by reference.

**Full audit results** (completed):

**`container_interface.tex`** — 9 occurrences:

| Line | Context | Action |
|---|---|---|
| 157 | Type alias table row: `vertex_reference_t<G>` | **Remove row** |
| 164 | Type alias table row: `edge_reference_t<G>` | **Remove row** |
| 468 | Edgelist type alias table row: `edge_reference_t<EL>` | **Remove or keep per Q3** |
| 498 | `contains_edge` default impl lambda: `[](edge_reference_t<EL> e)` | **Replace with `edge_t<EL> e`** |
| 511 | LaTeX comment: `[](edge_reference_t<EL>)` | **Update or remove** |
| 514 | Prose: "based on the `tuple` and `edge_info` types" | **Rename `edge_info` → `edge_data`** |
| 523 | Prose: "The `edge_info` patterns are" | **Rename `edge_info` → `edge_data`** |
| 525 | Pattern listing: `edge_info<integral,true,void,void>` | **Rename `edge_info` → `edge_data`** |
| 526 | Pattern listing: `edge_info<integral,true,void,scalar>` | **Rename `edge_info` → `edge_data`** |

**`D3130_Container_Interface/src/`** — 8 occurrences:

| File | Line | Context | Action |
|---|---|---|---|
| `concepts_edges.hpp:4` | `edge_reference_t<G> uv` in `targeted_edge` | **File will be rewritten** (Phase 2, task 5) |
| `concepts_edges.hpp:10` | `edge_reference_t<G> uv` in `sourced_edge` | **File will be rewritten** |
| `concepts_edges_before.hpp:4` | `edge_reference_t<G> uv` in `basic_targeted_edge` | **File will be removed** (Phase 2, task 9) |
| `concepts_edges_before.hpp:7` | `edge_reference_t<G> uv` in `basic_sourced_edge` | **File will be removed** |
| `concepts_edges_before.hpp:14` | `edge_reference_t<G> uv` in `targeted_edge` | **File will be removed** |
| `concepts_edges_before.hpp:18` | `edge_reference_t<G> uv` in `sourced_edge` | **File will be removed** |
| `concepts_target_edge_range.hpp:6` | `vertex_reference_t<G> u` | **File will be rewritten** (Phase 2, task 6) |
| `edgelist_types.hpp:11` | `using edge_reference_t = ...` | **Remove or keep per Q3** |

**Note:** 0 occurrences of `vertex_info` in any D3130 file. `edge_info` appears 4 times (all in prose, lines 514–526).

### Q3 — Unified Edge Descriptor Story (Edgelist + Adjacency List)

**Original question:** Should the edgelist keep `edge_reference_t<EL>` or align with the
adjacency list pattern?

**Decision: Remove `edge_reference_t<EL>`. Unify the descriptor story across both interfaces
so algorithms can work interchangeably with adjacency list edges and edge list edges.**

#### Background

The implementation has **two separate edge descriptor types** with incompatible designs:

| Aspect | `adj_list::edge_descriptor` | `edge_list::edge_descriptor` |
|---|---|---|
| Storage | Values (`size_t` indices or iterators) | References (`const VId&` + `reference_wrapper`) |
| Trivially copyable | Yes (RA case) | No (reference members) |
| Assignable | Yes | No (copy/move assignment deleted) |
| Needs graph for IDs | Yes (for `target_id`) | No (self-contained) |
| `source()`/`target()` (vertex descriptors) | Yes (`source()`) | No |
| Trait | `is_edge_descriptor_v` | `is_edge_list_descriptor_v` |
| sizeof (typical) | 16 bytes | 16 bytes (no EV), 24 bytes (with EV) |

The `edge<G, E>` concept gates on `is_edge_descriptor_v` and requires `source(g,e)` /
`target(g,e)` — both exclude edge list edges entirely. An algorithm written against
`edge<G, E>` silently rejects edge list edges.

#### Barriers to unification

1. **Type trait gate** — `edge<G,E>` checks `is_edge_descriptor_v`, which only matches
   `adj_list::edge_descriptor`. Edge list descriptors have a separate trait.

2. **`source(g,e)` / `target(g,e)` require a vertex container** — These return vertex
   descriptors. Edge lists have no vertex container to index into; they only know IDs.

3. **Reference semantics** — `edge_list::edge_descriptor` stores `const VId&` references,
   making it non-assignable and dangling-prone. Algorithms that store edges (e.g., priority
   queue for Kruskal's) would fail.

4. **Edge value in descriptor** — `edge_list::edge_descriptor<VId, EV>` carries the value
   directly. With a value-based redesign, `descriptor<size_t, double>` = 24 bytes, exceeding
   the 16-byte register-pair threshold.

5. **Edge value access asymmetry** — `adj_list` needs the graph to look up edge values;
   `edge_list` carries them directly. The shared CPOs handle dispatch, but semantics differ.

#### Recommended layered concept design

Define a lighter common concept both can satisfy, with adj_list refining it:

```
graph_edge<G, E>              ← common: source_id(g,e) + target_id(g,e)
  └── adj_list::edge<G, E>    ← refines: + is_edge_descriptor_v + source(g,e) + target(g,e)
```

Most algorithms only need vertex IDs (BFS, DFS, Dijkstra, Bellman-Ford, topological sort,
Kruskal's). Only algorithms that need to *navigate back to vertex data* require the full
`edge<G, E>` with `source(g,e)` / `target(g,e)`.

#### Paper changes required

| # | Change | Section |
|---|---|---|
| P1 | Remove `edge_reference_t<EL>` from edgelist type alias table and all usages | §5, §6 tables |
| P2 | Remove `edge_reference_t<EL>` from `edgelist_types.hpp` source file | §15 source files |
| P3 | Replace `[](edge_reference_t<EL> e)` lambda in `contains_edge` with `[](edge_t<EL> e)` | §4.5 function table |
| P4 | Define `graph_edge<G, E>` common concept: `requires(G& g, const E& e) { source_id(g, e) → convertible_to<vertex_id_t<G>>; target_id(g, e) → convertible_to<vertex_id_t<G>>; }` | New in §1 |
| P5 | Refine `edge<G, E>` to explicitly extend `graph_edge<G, E>` | §1.1 |
| P6 | Document the unified descriptor story — explain that `graph_edge` is the interop concept, `edge` is for adjacency-list-specific vertex navigation | §0 or new section |
| P7 | Document that edge list descriptors should be value types matching the pass-by-value convention | §0.2 / new edgelist descriptor section |
| P8 | Specify that `edge_list::edge_descriptor` stores only IDs (not the edge value), with value accessed via `edge_value(el, e)` against the container — keeping sizeof ≤ 16 bytes | Edgelist section |
| P9 | Add `is_edge_list_descriptor_v` trait to the traits table, or unify with `is_edge_descriptor_v` under a common `is_graph_edge_descriptor_v` | §3 traits |

#### Implementation changes required

| # | Change | File(s) |
|---|---|---|
| I1 | Redesign `edge_list::edge_descriptor` as a **value type**: store `VId source_id_; VId target_id_;` (values, not references). Drop `EV` template parameter — value accessed via `edge_value(el, e)` | `edge_list/edge_list_descriptor.hpp` |
| I2 | Make `edge_list::edge_descriptor` fully copyable, assignable, and trivially copyable (for integral VId). sizeof = `2 * sizeof(VId)` = 16 bytes for `size_t` | `edge_list/edge_list_descriptor.hpp` |
| I3 | Add `graph_edge<G, E>` concept (or whatever name is chosen) to a shared location | New file or `graph_concepts.hpp` |
| I4 | Update `edge<G, E>` to refine `graph_edge<G, E>` | `adj_list/adjacency_list_concepts.hpp` |
| I5 | Add `edge_list::edge_descriptor` support to `is_graph_edge_descriptor_v` (if unified trait chosen) or create `is_edge_list_descriptor_v` → `graph_edge`-satisfying trait | `edge_list/edge_list_traits.hpp` |
| I6 | Update `edge_cpo.hpp` shared CPO tiers to handle the value-based edge_list descriptor | `detail/edge_cpo.hpp` |
| I7 | Update `edge_value` CPO: for edge_list descriptors that no longer carry the value, add a tier that indexes back into the edge list container | `detail/edge_cpo.hpp` |
| I8 | Create an `edge_list::edge_descriptor_view` (optional) that wraps a plain edge range to produce `edge_list::edge_descriptor` objects, analogous to `adj_list::edge_descriptor_view` | New `edge_list/edge_list_descriptor_view.hpp` |
| I9 | Update edge_list concepts to work with the redesigned descriptor | `edge_list/edge_list.hpp` |

#### Open sub-questions

- **Naming: DECIDED — relax `edge<G,E>` by dropping the trait gate.** This makes `edge<G,E>`
  the general concept that works for both adjacency list and edge list edges. It becomes:
  `requires(G& g, const E& e) { source_id(g, e); target_id(g, e); }` — no
  `is_edge_descriptor_v` check. Any type that supports `source_id`/`target_id` via the CPOs
  qualifies, including plain tuples, `edge_data`, `edge_list::edge_descriptor`, and
  `adj_list::edge_descriptor`. Algorithms that additionally need vertex descriptors
  (`source(g,e)` / `target(g,e)`) add those requirements directly rather than relying on a
  trait gate. This is more general and avoids the need for a separate `graph_edge` concept.
  **Paper change P4 is superseded** — instead of adding `graph_edge`, relax `edge<G,E>`.
  **Implementation change I3 is superseded** — modify `edge<G,E>` in place rather than
  adding a new concept.
- **Edge value access: DECIDED — descriptor stores only an iterator (always, never an index).**
  Unlike `adj_list::edge_descriptor` which uses `conditional_t<RA, size_t, Iter>`, the
  `edge_list::edge_descriptor` **always** stores an iterator. The key insight is that edge
  list elements already carry both source and target IDs — there is nothing to cache. So the
  descriptor is just the iterator:

  ```cpp
  template <class EdgeIter>
  struct edge_descriptor {
      EdgeIter position_;  // 8 bytes — that's it
  };
  ```

  - `source_id(el, e)` → extracts from `*e.position_` (pattern dispatch: tuple get, member, etc.)
  - `target_id(el, e)` → extracts from `*e.position_`
  - `edge_value(el, e)` → extracts from `*e.position_`

  **Why always-iterator, not conditional like adj_list:**
  - `adj_list` caches index for RA containers because (1) vertex_id = index optimization,
    (2) index stability across graph mutation, (3) long-lived navigation handles stored in
    predecessor maps and priority queues. **None of these apply to edge lists.**
  - Edge lists are consumed immutably — descriptors are transient iteration handles that live
    only for the current loop iteration, never stored across container mutations.
  - Always-iterator = **1 field, 8 bytes, 1 code path** vs 3 fields, 24 bytes, 2 code paths.

  **User impact: None.** Users interact exclusively through CPOs (`source_id`, `target_id`,
  `edge_value`). They never construct descriptors (views produce them), never inspect internal
  storage, and never see the sizeof. Code is identical for both adj_list and edge_list edges:
  ```cpp
  template <class G, class E> requires edge<G, E>
  void process(G& g, E e) { auto sid = source_id(g, e); }  // works for both
  ```

  **The one theoretical edge case:** Storing an edge_list descriptor across a
  `vector::push_back` on the edge list would dangle (iterator invalidation). But this is the
  same behavior adj_list has for non-RA edge containers, and it's a misuse pattern — you
  shouldn't store iteration-produced descriptors across mutations.

  **Implementation change I1 revised:** `edge_list::edge_descriptor<EdgeIter>` stores only
  `EdgeIter position_` (8 bytes, trivially copyable, fully assignable). No `VId` members.
  **Implementation change I7 revised:** All edge_list CPO tiers dereference the iterator to
  extract IDs and values — no stored position indexing needed.
- **`edge_list::edge_descriptor_view`: DECIDED — No view. `edges(el)` returns the raw range.**
  A wrapping view would add complexity, a lifetime dependency, and break structured bindings
  with zero user-visible benefit through the CPO interface.

  **Why no view is needed (unlike adj_list):**
  - `adj_list::edge_descriptor_view` exists because per-vertex edges don't know their source
    vertex — the view injects that context. Edge list elements already carry both endpoints.
    There is nothing to wrap.
  - Since we dropped the trait gate on `edge<G,E>`, the concept is just `source_id(g,e)` +
    `target_id(g,e)`. Plain tuples, pairs, and `edge_data` already satisfy this through the
    CPO's lower-priority tiers (tuple_like, pair, data member). No descriptor wrapping needed.

  **What you lose with a view (flexibility cost):**
  1. **Self-contained values** — Raw elements can be copied, sorted, stored in priority queues
     freely (e.g., Kruskal's sorts edges by weight). A descriptor would create iterator
     lifetime dependencies.
  2. **Structured bindings** — `auto [src, tgt, val] = e;` works out of the box with tuples
     and pairs. A descriptor view would break this unless tuple protocol is added.
  3. **User data passthrough** — Custom edge structs with extra fields are directly accessible.
     A descriptor erases the element type behind an iterator.

  **User-visible behavior:** Identical through CPOs either way. The only difference is
  `edge_t<EL>` is the user's raw element type (tuple, pair, `edge_data`, custom struct)
  rather than a descriptor — which is actually **better** for edge lists since users expect
  to work with their own data types directly.

  **Implementation changes I1, I8 superseded:** No `edge_list::edge_descriptor` class or
  `edge_list::edge_descriptor_view` needed. Edge list edges are the raw range elements.
  CPO dispatch handles all supported patterns (tuple, pair, `edge_data`, custom structs)
  directly.

### Q4 — Adjacency Matrix Support
**Question:** The LaTeX includes `define_adjacency_matrix<G>`, `is_adjacency_matrix<G>`,
and `adjacency_matrix<G>` traits/concepts, and `contains_edge` has a special constant-time
implementation for adjacency matrices. These are **all removed** from the implementation.
Should they remain in the proposal as planned/future, or be dropped entirely?

**Decision: Keep the adjacency matrix traits/concepts in the paper.**

When an adjacency matrix is used, the traits enable algorithms to assume constant-time
random access for `contains_out_edge(g, u, v)` and `contains_out_edge(g, uid, vid)`.
Without the trait, algorithms must conservatively assume linear-time edge lookup, even when
the underlying container supports O(1) access.

**What to include:**
- `define_adjacency_matrix<G>` — user-specializable trait to declare a graph is an adjacency matrix
- `is_adjacency_matrix<G>` / `is_adjacency_matrix_v<G>` — query trait
- `adjacency_matrix<G>` — concept combining the trait with `index_adjacency_list<G>`
- `contains_out_edge` constant-time guarantee when `adjacency_matrix<G>` is satisfied

**Note:** These are not yet in the implementation. The paper should include them with a note
that the reference implementation has not yet added adjacency matrix support, but the
traits/concepts are designed to be ready when it does. The implementation will need to add
these back.

### Q5 — `raw_vertex_id_t<G>` Inclusion
**Question:** The implementation defines `raw_vertex_id_t<G>` which preserves reference
qualifiers on the vertex ID (important for map-based graphs where vertex IDs are const
references to keys). Should this be in the proposal?

**Decision: Make `raw_vertex_id_t` exposition-only; make `vertex_id_store_t` normative.**

`raw_vertex_id_t<G>` is unusual for a standard library — it's a "raw decltype" alias
(preserves references) used only as a metaprogramming stepping stone. The standard doesn't
expose such aliases elsewhere. However, the optimization it enables is genuinely important.

**How it works:**
- `vertex_id_t<G>` = `remove_cvref_t<decltype(vertex_id(g, u))>` — always a clean value type
- `raw_vertex_id_t<G>` = `decltype(vertex_id(g, u))` — preserves `const string&` for maps

**Its sole consumer is `vertex_id_store_t<G>`** (in `traversal_common.hpp`):
```cpp
using vertex_id_store_t = conditional_t<
    is_reference_v<raw_vertex_id_t<G>>,
    reference_wrapper<remove_reference_t<raw_vertex_id_t<G>>>,
    vertex_id_t<G>>;
```
This avoids copying expensive vertex IDs (e.g., `string` map keys) into algorithm data
structures by storing a `reference_wrapper` instead. For integral IDs (`size_t`) it's a
no-op — `vertex_id_store_t<G>` = `vertex_id_t<G>`.

**Options considered:**

| Option | Approach | Verdict |
|---|---|---|
| A — Include both | Exposes unusual "raw decltype" alias | Too unusual for standard |
| **B — exposition-only `raw_vertex_id_t`, normative `vertex_id_store_t`** | User gets the optimization; internal metaprogramming hidden | **Selected** |
| C — Drop both, mandate cheap IDs | Simplest but excludes `map<string,...>` graphs | Too restrictive |
| D — Drop both, rely on `auto` | Works but loses named type for docs/concepts | Insufficient for specification |

**In the paper:**
- *`raw-vertex-id-type`* (italic, exposition-only per WG21 convention)
- `vertex_id_store_t<G>` — normative public alias, documented as "the type algorithms use to
  store vertex IDs efficiently"
- Matches precedent: `std::optional` has exposition-only *`val`* but public `value()`

### Q6 — Edgelist Namespace Merge
**Question:** The LaTeX has a `\phil` note about wanting to merge edgelist into `std::graph`
but having difficulty. The implementation uses `graph::edge_list`. Should the proposal keep
separate namespaces or attempt to merge?

**Decision: Keep separate namespaces. Use `std::graph::edge_list`.**

Rationale:
1. **Type alias collisions.** `vertex_id_t<G>` (adj_list) and `vertex_id_t<EL>` (edge_list)
   have different definitions. Template aliases can't be disambiguated by concepts, so they
   must live in separate namespaces.
2. **Different abstractions.** Adjacency lists organize edges per-vertex (graph structure).
   Edge lists are flat ranges (data interchange). Separate namespaces make this explicit.
3. **Shared CPOs work already.** `source_id`, `target_id`, `edge_value` live in `graph::`
   and dispatch to the right tier via type traits — cross-namespace sharing is solved.
4. **Matches the implementation.** `graph::adj_list` and `graph::edge_list` work well in
   practice. The `\phil` note was written before the collision problem was understood.

**Paper action:** Update `std::graph::edgelist` → `std::graph::edge_list`. Remove the
`\phil` note on line 417 of `container_interface.tex`.

### Q7 — `vertex_property_map` Scope
**Question:** Is `vertex_property_map` part of the GCI (D3130) or should it be in the
Algorithms paper (D3128) or a separate utility paper? It is used by algorithms but defined
alongside the graph interface.

**Decision: Include `vertex_property_map` in the GCI (D3130).**

Rationale:
1. **Defined by container properties.** `vertex_property_map<G, T>` resolves to `vector<T>`
   for index graphs or `unordered_map<vertex_id_t<G>, T>` for mapped graphs. That decision
   depends entirely on GCI concepts (`index_vertex_range<G>` vs `mapped_vertex_range<G>`).
2. **Algorithms consume, not define.** BFS/DFS/Dijkstra *use* property maps for colors,
   distances, predecessors — but the map's type and construction depend on the container, not
   the algorithm. Putting it in D3128 would create a circular dependency.
3. **Parallels `vertex_id_store_t<G>`** (Q5) — both are type aliases that adapt to the
   graph's container strategy. Both belong in the GCI.
4. **Custom containers need it.** Users implementing new graph containers satisfying GCI
   concepts should get `vertex_property_map` working automatically — only possible if it's
   defined alongside the concepts it depends on.

### Q8 — Source CPO Default Behavior
**Question:** The LaTeX marks `source_id(g, uv)` as "n/a, optional". In the implementation,
`source_id` always works on edge descriptors (because `edge_descriptor` stores the source
vertex). Is `source_id` still "optional" or has it become mandatory for any edge_descriptor?

**Decision: `source_id` is effectively mandatory.**

Every `edge_descriptor` stores the source vertex, so `source_id(g, uv)` resolves
automatically for any graph whose edges are descriptors. The paper should drop the
"n/a, optional" qualifier and list `source_id` alongside `target_id` as a supported CPO for
descriptor-based edges. For custom edge types that do not use descriptors, the user must still
provide an override — but that is a general CPO customisation rule, not a special case.

---

## 15. Source File Reorganization

The LaTeX includes source files from `D3130_Container_Interface/src/` via `\lstinputlisting`.
These need to be rewritten:

| LaTeX Source File                     | Status      | Action                                           |
| ------------------------------------- | ----------- | ------------------------------------------------ |
| `concepts_edges.hpp`                  | Obsolete    | Replace with single `edge<G, E>` concept         |
| `concepts_edges_before.hpp`           | Obsolete    | Remove (basic variants eliminated)               |
| `concepts_target_edge_range.hpp`      | Obsolete    | Replace with `out_edge_range<R, G>`              |
| `concepts_vertex_range.hpp`           | Outdated    | Rewrite: add `vertex<G,V>`, update `index_vertex_range`, add `mapped_vertex_range` |
| `concepts_adj_list.hpp`               | Outdated    | Rewrite: remove sourced/basic variants, add bidirectional/mapped |
| `concepts_basic_adj_list.hpp`         | Obsolete    | Remove entirely                                  |
| `descriptor.hpp`                      | Obsolete    | Replace with split descriptors (vertex + edge)   |
| `descriptor_view.hpp`                 | Outdated    | Replace with vertex_descriptor_view + edge_descriptor_view |
| `edgelist_concepts.hpp`               | Outdated    | Update to 2-arg CPO form, rename concepts        |
| `edgelist_types.hpp`                  | Outdated    | Update to 2-arg form, add `raw_vertex_id_t`      |
| `edgelist_typetraits.hpp`             | Minimal     | Same                                             |
| (new) `descriptor_traits.hpp`         | Missing     | Add descriptor trait types and concepts           |
| (new) `value_concepts.hpp`            | Missing     | Add `vertex_value_function`, `edge_value_function` |

---

## 16. Prioritized Task List

### Phase 0: Decision Summary (All Resolved — No File Edits)

All eight open questions are resolved. This phase is a reference checklist only.

- **Q1 — Descriptor exposition:** Hybrid (b)+(c). Concepts normative, descriptor classes
  with exposition-only internals, pattern-detection concepts and traits fully specified.
- **Q2 — Passing convention:** Pass descriptors by value. Full prose audit completed
  (17 occurrences catalogued in Q2). Execute replacements in Phases 3–4.
- **Q3 — Unified edge story:** Drop `is_edge_descriptor_v` trait gate on `edge<G,E>`.
  No `edge_list::edge_descriptor` class. No `edge_list::edge_descriptor_view`. Raw edge list
  elements satisfy `edge<G,E>` directly through CPO tier dispatch.
- **Q4 — Adjacency matrix:** Keep traits/concepts in paper (`define_adjacency_matrix`,
  `is_adjacency_matrix`, `adjacency_matrix<G>`, `contains_out_edge` O(1) guarantee).
  Note reference implementation has not yet added support.
- **Q5 — `raw_vertex_id_t`:** Exposition-only (*`raw-vertex-id-type`*).
  `vertex_id_store_t<G>` normative.
- **Q6 — Namespace:** Separate namespaces — `std::graph::edge_list`. Remove `\phil` merge
  note.
- **Q7 — `vertex_property_map`:** Include in D3130 (GCI). Defined by container properties,
  parallels `vertex_id_store_t`.
- **Q8 — `source_id` optionality:** Effectively mandatory for descriptor-based edges. Drop
  "n/a, optional" qualifier. Custom non-descriptor edge types still need user override.

### Phase 1: Descriptor Architecture (Foundation — Must Be First)

1. **Rewrite descriptor.hpp** — Replace single `descriptor<InnerIter>` with split
   `vertex_descriptor<VertexIter>` + `edge_descriptor<EdgeIter, VertexIter, EdgeDirection>`.
   Use exposition-only data members per Q1.
2. **Rewrite descriptor_view.hpp** — Replace `descriptor_view` / `descriptor_subrange_view`
   with `vertex_descriptor_view` + `edge_descriptor_view`.
3. **Add descriptor_traits.hpp** — Type traits and concepts for descriptor detection.
4. **Update descriptor prose** — Rewrite "Vertex and Edge Descriptor Views" section. Include
   "Why Descriptors?" rationale (§0.2a). Explain pass-by-value convention per Q2.

### Phase 2: Concept Overhaul

5. **Rewrite `edge<G, E>`** — Replace 6 edge concepts with single relaxed `edge<G, E>`:
   `requires(G& g, const E& e) { source_id(g, e); target_id(g, e); }`. No
   `is_edge_descriptor_v` trait gate (Q3). Both adj_list descriptors and raw edge list
   elements satisfy this concept.
6. **Rewrite edge range concepts** — Replace `targeted_edge_range` /
   `basic_targeted_edge_range` with `out_edge_range<R, G>` + `in_edge_range<R, G>`.
7. **Rewrite vertex concepts** — Add `vertex<G, V>`, update `vertex_range`,
   `index_vertex_range`, add `mapped_vertex_range`.
8. **Rewrite adjacency list concepts** — Remove all `basic_*` / `sourced_*` variants, add
   `bidirectional_adjacency_list`, `mapped_adjacency_list`, etc.
9. **Remove obsolete concept source files** — `concepts_edges_before.hpp`,
   `concepts_basic_adj_list.hpp`.

### Phase 3: Type Aliases and Traits

10. **Update type alias table** — Replace `vertex_reference_t` → `vertex_t` (descriptor),
    `edge_reference_t` → `edge_t` (descriptor); remove `graph_reference_t`; add
    `out_edge_range_t`, `out_edge_iterator_t`, `out_edge_t`, `in_edge_range_t`,
    `in_edge_iterator_t`, `in_edge_t`.
11. **Update vertex ID aliases** — Add `vertex_id_store_t<G>` as normative. Add
    *`raw-vertex-id-type`* as exposition-only (Q5).
12. **Update traits table** — Remove `unordered_edge` / `ordered_edge` traits. Keep
    adjacency matrix traits (Q4): `define_adjacency_matrix<G>`, `is_adjacency_matrix<G>` /
    `_v`, `adjacency_matrix<G>`. Add bidirectional query traits, combined query traits.
13. **Update `has_contains_edge`** — Add second template parameter V.

### Phase 4: Function Tables and CPOs

14. **Rename primary CPOs** — `edges` → `out_edges` (with alias notes), `degree` →
    `out_degree`, `find_vertex_edge` → `find_out_edge`, `contains_edge` →
    `contains_out_edge`, `has_edge` → `has_edges`.
15. **Add incoming-edge CPOs** — `in_edges`, `in_degree`, `find_in_edge`,
    `contains_in_edge`.
16. **Update `source_id` / `source` CPOs** — `source_id(g, uv)` is mandatory for
    descriptor-based edges (Q8). Drop "n/a, optional" qualifier. Add `source(g, uv)` CPO
    returning vertex descriptor.
17. **Execute Q2 audit replacements** — Replace `edge_reference_t<G>` → `edge_t<G>` and
    `vertex_reference_t<G>` → `vertex_t<G>` in all function parameters, concept definitions,
    and signatures (17 occurrences catalogued in Q2). Pass descriptors by value.

### Phase 5: Edgelist Updates

18. **Update edgelist concepts** — Switch to 2-arg CPO form. Edge list edges are raw range
    elements (tuples, pairs, `edge_data`, custom structs) — no `edge_list::edge_descriptor`
    class or view needed (Q3). Edges satisfy `edge<G,E>` via CPO tier dispatch.
19. **Remove `edge_reference_t<EL>`** — From edgelist type alias table and
    `edgelist_types.hpp` source file. Replace `[](edge_reference_t<EL> e)` lambda in
    `contains_edge` with the raw element type.
20. **Update edgelist type aliases** — 2-arg form, add `raw_vertex_id_t`.
21. **Update edgelist function table** — 2-arg for `source_id`, `target_id`, `edge_value`.
22. **Rename edgelist patterns** — `edge_info` → `edge_data`, `vertex_info` → `vertex_data`
    (renamed to reflect data model role). Add supported patterns.
23. **Update namespace** — `std::graph::edgelist` → `std::graph::edge_list` (Q6). Remove
    `\phil` merge note (line 417).

### Phase 6: New Sections

24. **Add `vertex_data` / `edge_data` / `neighbor_data`** — New classes and structs section.
25. **Add value function concepts** — `vertex_value_function`, `edge_value_function`.
26. **Add `vertex_property_map`** — New GCI subsection (Q7). Show resolution to `vector<T>`
    for index graphs and `unordered_map<vertex_id_t<G>, T>` for mapped graphs.
27. **Update vertex_id determination** — Descriptor-based approach.

### Phase 7: Prose and Cleanup

28. **Audit all prose** — Search-and-replace: `vertex_reference_t` → `vertex_t`,
    `edge_reference_t` → `edge_t`, `vertex_info` → `vertex_data`, `edge_info` → `edge_data`,
    `targeted_edge` → `edge<G,E>`, `sourced_edge` → (remove), `basic_*` → (remove) in
    explanatory text.
29. **Update adjacency matrix prose** — Revise `contains_out_edge` discussion to match Q4
    decision (keep traits/concepts, note future implementation). Remove stale traits
    discussion that assumed removal.
30. **Review "Using Existing Data Structures"** section for accuracy.
31. **Update revision history**.
32. **Cross-reference with D3128 (Algorithms), D3129 (Views), D3127 (Terminology)** — Ensure
    consistent type names and concept names across all papers.

---

## 17. Summary of Removed Items

| Item                           | Was in LaTeX as                              |
| ------------------------------ | -------------------------------------------- |
| `vertex_reference_t<G>`        | Type alias — **replaced by `vertex_t<G>`** (descriptor) |
| `edge_reference_t<G>`          | Type alias, used in concept parameters — **replaced by `edge_t<G>`** (descriptor) |
| `graph_reference_t<G>`         | Type alias                                   |
| `vertex_info`                  | **Renamed to `vertex_data`** — represents the vertex data model |
| `edge_info`                    | **Renamed to `edge_data`** — represents the edge data model |
| `targeted_edge`                | Concept                                      |
| `sourced_edge`                 | Concept                                      |
| `sourced_targeted_edge`        | Concept                                      |
| `basic_targeted_edge`          | Concept                                      |
| `basic_sourced_edge`           | Concept                                      |
| `basic_sourced_targeted_edge`  | Concept                                      |
| `targeted_edge_range`          | Concept                                      |
| `basic_targeted_edge_range`    | Concept                                      |
| `_common_vertex_range`         | Concept                                      |
| `basic_adjacency_list`         | Concept                                      |
| `basic_index_adjacency_list`   | Concept                                      |
| `basic_sourced_adjacency_list` | Concept                                      |
| `basic_sourced_index_adjacency_list` | Concept                                |
| `sourced_adjacency_list`       | Concept                                      |
| `sourced_index_adjacency_list` | Concept                                      |
| `define_unordered_edge<G>`     | Trait                                        |
| `unordered_edge<G>`            | Concept/trait                                |
| `ordered_edge<G>`              | Concept/trait                                |
| `descriptor<InnerIter>`         | Single monolithic descriptor class           |
| `descriptor_view(R&&)`          | Factory function                             |
| `descriptor_subrange_view(R&&, R&&)` | Factory function                        |

## 18. Summary of New Items

| Item                                     | Category         |
| ---------------------------------------- | ---------------- |
| `vertex_descriptor<VertexIter>`          | Class            |
| `edge_descriptor<EdgeIter, VertexIter, EdgeDirection>` | Class |
| `vertex_descriptor_view<VertexIter>`     | View class       |
| `edge_descriptor_view<EdgeIter, VertexIter, EdgeDirection>` | View class |
| `out_edge_tag` / `in_edge_tag`          | Tag types        |
| `edge<G, E>` / `vertex<G, V>`           | Concepts         |
| `out_edge_range<R, G>` / `in_edge_range<R, G>` | Concepts  |
| `ordered_vertex_edges<G>`                | Concept          |
| `bidirectional_adjacency_list<G>`        | Concept          |
| `index_bidirectional_adjacency_list<G>`  | Concept          |
| `hashable_vertex_id<G>`                  | Concept          |
| `mapped_vertex_range<G>`                 | Concept          |
| `mapped_adjacency_list<G>`               | Concept          |
| `mapped_bidirectional_adjacency_list<G>` | Concept          |
| `vertex_descriptor_type<T>` / `edge_descriptor_type<T>` | Concepts |
| `descriptor_type<T>` / `random_access_descriptor<T>` / `iterator_based_descriptor<T>` | Concepts |
| All descriptor traits (`is_vertex_descriptor<T>`, etc.) | Type traits |
| Edge/vertex pattern detection concepts   | Concepts         |
| `out_edge_range_t<G>`, `out_edge_iterator_t<G>`, `out_edge_t<G>` | Type aliases |
| `in_edge_range_t<G>`, `in_edge_iterator_t<G>`, `in_edge_t<G>` | Type aliases |
| *`raw-vertex-id-type`* (exposition-only)  | Type alias (Q5)  |
| `vertex_id_store_t<G>`                   | Type alias (Q5)  |
| `in_edges(g, u)` / `in_edges(g, uid)`   | CPO              |
| `in_degree(g, u)` / `in_degree(g, uid)` | CPO              |
| `source(g, uv)`                          | CPO              |
| `find_in_edge(g, ...)` (3 overloads)    | CPO              |
| `contains_in_edge(g, ...)` (2 overloads)| CPO              |
| `has_in_degree<G>` / `_v`               | Trait            |
| `has_find_in_edge<G>` / `_v`            | Trait            |
| `has_contains_in_edge<G, V>` / `_v`     | Trait            |
| `has_basic_queries<G>` / `_v`           | Trait            |
| `has_full_queries<G>` / `_v`            | Trait            |
| `vertex_data<VId, V, VV>`               | Struct (8 specs) |
| `edge_data<VId, Sourced, E, EV>`        | Struct (16 specs)|
| `neighbor_data<VId, Sourced, V, VV>`    | Struct           |
| `copyable_vertex_t` / `copyable_edge_t` / `copyable_neighbor_t` | Aliases |
| `vertex_value_function<VVF, G, V>`      | Concept          |
| `edge_value_function<EVF, G, E>`        | Concept          |
| `vertex_property_map<G, T>`             | Type alias (Q7)  |
| `graph_error`                            | Exception class  |
| `vertex_property_map<G, T>`             | Type alias       |
| `make_vertex_property_map(g, ...)`       | Factory functions |
| `vertex_property_map_contains(m, uid)`   | Utility function |
| `vertex_property_map_get(m, uid, dflt)`  | Utility function |
