# D3130 Container Interface — Review Comments

Review of `D3130_Container_Interface/tex/container_interface.tex` against the
reference implementation in `/home/phil/dev_graph/graph-v3/include/graph/`.

Scope reviewed: full GCI paper text — adjacency-list concepts/traits/types/functions,
descriptors, vertex property map, value-function concepts, partitions, and the
edgelist interface. Focus is clarity, consistency (paper vs. code), and completeness.
Topics owned by sibling papers (algorithms, views, containers) are noted but not
re-reviewed here.

Legend: **[HIGH]** = code/paper genuinely disagree on the interface; **[MED]** =
documented symbol/behavior absent or different in the reference impl; **[LOW]** =
wording/snippet polish.

---

## 1. Code-vs-paper inconsistencies

### 1.1 [HIGH] `edge<G,E>` concept — RESOLVED via `basic_edge`/`edge` split
- Original mismatch: paper required `source_id`/`source`/`target_id`/`target`;
  code required only `source_id`/`target_id`.
- Resolution (implemented in code + paper): introduced a shared
  `graph::basic_edge<G,E>` concept (ids only) and redefined the adjacency-list
  `adj_list::edge<G,E>` to refine `basic_edge` plus `source(g,e)`/`target(g,e)`.
  `edge_list::basic_sourced_edgelist` now refines `basic_edge`, tying both ADTs
  to one shared edge floor. Edge-list elements (raw tuples/pairs/`edge_data`)
  satisfy `basic_edge` but not `adj_list::edge` (no vertex container to resolve
  descriptors). Verified: adj_list / container / views / edge_list suites pass;
  edge_list interop test updated to assert the new contract.

### 1.2 [HIGH] `edge_descriptor::edge_storage_type` — RESOLVED (paper now matches code)
- Original mismatch: paper showed
  `edge_storage_type = conditional_t<random_access_iterator<EdgeIter>, size_t, EdgeIter>`
  with prose implying an index for random-access edges; code always uses
  `using edge_storage_type = EdgeIter;` (no index-only edge path, since edges
  are always backed by a physical container element).
- Resolution (paper updated): `src/descriptor.hpp` snippet now shows
  `edge_storage_type = EdgeIter`; the descriptor-storage prose states the edge
  is always stored as its iterator; the comparison-table size row and the
  copy-size paragraph now scope the "one integer" claim to the index-based
  `vertex_descriptor` and describe an `edge_descriptor` as an iterator plus the
  source `vertex_descriptor`. PDF rebuilds cleanly.

### 1.3 [MED] Edgelist `vertex_id_t<EL>`: paper said `target_id` — RESOLVED (paper aligned to `source_id`)
- Original mismatch: paper's type-alias table and namespace prose derived the
  edgelist `vertex_id_t<EL>` from `target_id(el,uv)`; code derives both
  `vertex_id_t` and `raw_vertex_id_t` from `source_id(el,uv)`.
- Resolution (paper updated to match code): the "Edgelist Interface Type
  Aliases" table now shows `vertex_id_t<EL> = remove_cvref_t<decltype(source_id(el,uv))>`
  and `raw-vertex-id-type = decltype(source_id(el,uv))`; the namespace prose now
  says the edgelist `vertex_id_t` is derived from `source_id(el,uv)`. (Both edge
  ids represent vertices of the same set, so either is a valid representative;
  `source_id` is chosen to match the implementation.) PDF rebuilds cleanly.

### 1.4 [MED] `vertex_id_store_t<G>` does not exist — RESOLVED (removed from paper)
- Original mismatch: the type-alias table listed `vertex_id_store_t<G>` (labeled
  "normative") with a "(see below)" definition that was never actually defined;
  no such alias exists in the code.
- Resolution (paper updated): removed the `vertex_id_store_t<G>` row from the
  "Graph Container Interface Type Aliases" table and the stale mention in
  `revision.tex`. The valid `raw-vertex-id-type` exposition-only alias (which
  maps to the code's `raw_vertex_id_t<G>`) is retained. PDF rebuilds cleanly.

### 1.5 [MED] Adjacency-matrix trait family — RESOLVED (flagged not-yet-implemented)
- Paper: traits table listed `define_adjacency_matrix`, `is_adjacency_matrix`,
  `is_adjacency_matrix_v`, and the `adjacency_matrix` concept; the
  `contains_out_edge` default described a constant-time path guarded by
  `is_adjacency_matrix_v<G>`. None exist in the code; the actual default always
  does a linear `find_if`.
- Resolution (paper updated): added a group note to the traits table marking the
  whole adjacency\_matrix family as "not yet in the reference implementation,"
  and a note after the Edge Functions table that the constant-time
  `contains_out_edge` branch is likewise future (the linear default is always
  used until then). Kept as documented future design per author direction.

### 1.6 [MED] Partition-filtered `out_edges(g,*,pid)` — RESOLVED (flagged not-yet-implemented)
- Paper stated `out_edges(g,uid,pid)` / `out_edges(g,u,pid)` filter edges by
  target partition; no such overload exists in the code (only `vertices(g,pid)`,
  `num_vertices(g,pid)`, `num_partitions(g)`, `partition_id(g,u)`).
- Resolution (paper updated): the partitions section now states these
  partition-filtered `out_edges` overloads are not yet in the reference
  implementation. Kept as documented future API per author direction.

### 1.7 [MED] Edgelist functions `num_edges/has_edges/contains_edge` — RESOLVED (flagged not-yet-implemented)
- Paper documented `num_edges(el)`, `has_edges(el)`, `contains_edge(el,uid,vid)`
  with default implementations; the code marks these "(todo)" and implements
  only `source_id`/`target_id`/`edge_value`.
- Resolution (paper updated): added a note after the Edgelist Interface
  Functions table that these three functions are not yet in the reference
  implementation while the id/value CPOs are available now. Kept as documented
  target interface per author direction.

### 1.8 [LOW] `vertex_value(g,uid)` convenience overload — RESOLVED (added to code)
- Original mismatch: paper's vertex-functions table documented
  `vertex_value(g,uid)` (default `vertex_value(g,*find_vertex(g,uid))`); the
  `_vertex_value` CPO provided only the descriptor form `operator()(g, u)`.
- Resolution (code updated to match paper): added a `uid` convenience overload
  to the `_vertex_value` CPO. It mirrors the descriptor dispatch — preferring a
  member `g.vertex_value(uid)` or ADL `vertex_value(g, uid)` that takes the id
  directly, and only falling back to `vertex_value(g, *find_vertex(g, uid))`
  when neither exists — via a dedicated `_Choose_uid` strategy. Added tests for
  the find_vertex fallback and for id-taking member priority; adj_list (27
  vertex_value cases) and container suites pass.

### 1.9 [LOW] `mapped_vertex_range` snippet omits the `hashable_vertex_id` requirement
- Paper: `src/concepts_vertex_range.hpp` shows `mapped_vertex_range` as
  `!index_vertex_range<G> && requires { vertices(g) -> forward_range; find_vertex(g,uid); }`.
- Code: also requires `hashable_vertex_id<G>` (vertex id must be usable as an
  `unordered_map` key). The `hashable_vertex_id` concept is not mentioned in the
  paper at all.
- Action: add `hashable_vertex_id<G>` to the snippet and a one-line description,
  or note why it is intentionally elided.

### 1.10 [LOW] `vertex<G,V>` snippet omits the `is_vertex_descriptor_v` guard
- Paper: `src/concepts_vertex_range.hpp` `vertex` concept requires
  `vertex_id(g,u)` and `find_vertex(g,uid)`.
- Code: additionally requires `is_vertex_descriptor_v<remove_cvref_t<V>>`.
- Action: add the guard to the snippet (minor, but it is load-bearing for
  disambiguating descriptors from raw values).

---

## 2. Clarity / completeness observations

### 2.1 [LOW] Partition default-implementation semantics understated
- Paper defaults: `vertices(g,pid)` → `vertices(g)`; `num_vertices(g,pid)` →
  `size(vertices(g))`.
- Code defaults: return the full range/count **only when `pid==0`**, otherwise
  an empty range / `0`. This matches the "one partition holds everything" model
  but the table omits the `pid==0` condition, which could mislead implementers.
- Action: state the defaults as "for `pid==0`, returns all vertices; otherwise
  empty / 0".

### 2.2 [LOW] Undocumented `num_edges(g,u)` overload
- Code has a `num_edges(g, u)` (per-vertex) CPO in addition to `num_edges(g)`.
  The paper documents only `num_edges(g)` (and `out_degree`). If `num_edges(g,u)`
  is intended as public API, document it or alias it to `out_degree`; if it is
  internal, no change needed.

### 2.3 Concept count wording
- Paper: "Six concepts are defined" for the adjacency list — accurate for the
  documented set. Code additionally defines `ordered_vertex_edges` (used by
  triangle-count in the algorithms paper) and `hashable_vertex_id` (used by
  `mapped_vertex_range`). `ordered_vertex_edges` is reasonably left to the
  algorithms paper; `hashable_vertex_id` is GCI-local and probably belongs here
  (see 1.9).

---

## 3. Confirmed consistent (spot-checked, no action)
- Adjacency-list concepts: `adjacency_list`, `index_adjacency_list`,
  `bidirectional_adjacency_list`, `index_bidirectional_adjacency_list`,
  `mapped_adjacency_list`, `mapped_bidirectional_adjacency_list` — present and
  matching.
- Query traits: `has_degree`, `has_find_vertex`, `has_find_vertex_edge`,
  `has_contains_edge`, `has_in_degree`, `has_find_in_edge`, `has_contains_in_edge`,
  `has_basic_queries`, `has_full_queries` — present, definitions match.
- `find_out_edge` primary with `find_vertex_edge` alias; `contains_out_edge`
  primary with `contains_edge` alias — matches paper; both re-exported into
  `graph::`.
- `vertex_property_map<G,T>` and the four helpers
  (`make_vertex_property_map` eager+lazy, `vertex_property_map_contains`,
  `vertex_property_map_get`) — match exactly.
- `vertex_data` / `edge_data` / `neighbor_data` specialization tables and the
  `copyable_vertex_t` / `copyable_edge_t` / `copyable_neighbor_t` aliases —
  match `graph_data.hpp`.
- `descriptor_traits.hpp` (`is_vertex_descriptor`, `is_edge_descriptor`,
  `is_descriptor` + `_v` forms) — match.
- `vertex_descriptor` storage conditional (`size_t` vs iterator), `value()`,
  `vertex_id()`, `underlying_value()`, `inner_value()`, `operator++` — match.
- `out_edge_tag` / `in_edge_tag`, `is_in_edge` / `is_out_edge` on
  `edge_descriptor` — match.
- Partition CPOs `num_partitions(g)`, `partition_id(g,u)`, `vertices(g,pid)`,
  `num_vertices(g,pid)` — present.
- `graph_error : runtime_error` — matches.
- Edgelist concepts `basic_sourced_edgelist`, `basic_sourced_index_edgelist`,
  `has_edge_value`, and the `is_directed`/`is_directed_v` trait — match.

---

## 4. Open questions (scope / intent — need author input)

- **Q1 (adjacency matrix):** RESOLVED — kept as future design, flagged "not yet
  in the reference implementation" in the traits table and Edge Functions note.
  See 1.5.
- **Q2 (partition edge filtering):** RESOLVED — kept as future API, flagged not
  yet implemented in the partitions section. See 1.6.
- **Q3 (edgelist functions):** RESOLVED — kept as target interface, flagged not
  yet implemented after the Edgelist Functions table. See 1.7.
- **Q4 (edgelist `vertex_id`):** RESOLVED — paper aligned to `source_id` (the
  code's choice). See 1.3.
- **Q5 (`hashable_vertex_id`):** Does the `hashable_vertex_id` concept belong in
  this paper (it is GCI-local and underpins `mapped_vertex_range`), or is it
  intentionally hidden? (See 1.9.)
