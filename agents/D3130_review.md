# D3130 Container Interface — Review Comments

Review of `D3130_Container_Interface/tex/container_interface.tex` against the
reference implementation in `/home/phil/dev_graph/graph-v3/include/graph/`.

---

## Fresh review pass (2026-06-08)

Re-verified the entire paper against the current code. Concepts, traits, type
aliases, descriptors, CPO defaults, value-function concepts, property map, data
structs (`vertex_data`/`edge_data`/`neighbor_data` + `copyable_*` aliases), and the
edgelist interface all match. Items resolved this pass (author-approved):

- **[GAP] `ordered_vertex_edges<G>` now documented.** The concept lives in GCI code
  (`adjacency_list_concepts.hpp`, `graph::adj_list`) but was undocumented (paper said
  "Six concepts"). Added it to the snippet `concepts_adj_list.hpp` and to the
  Adjacency List Concepts section as a seventh, orthogonal (semantic-ordering) concept,
  cross-referencing triangle counting in the Algorithms paper.
- **[GAP] `vertex_property_map_value_t` + `vertex_property_map_for` now documented.**
  Both are public in `vertex_property_map.hpp` (re-exported into `graph`) but the paper
  documented only the alias + 4 helpers. Added an `lstlisting` and prose to the Vertex
  Property Map section.
- **[GAP] Index-only descriptor types named.** The descriptor section described
  index-only vertices (compressed_graph) in prose; now names `index_iterator`
  (`iota_view<size_t,size_t>` iterator) and `index_vertex_descriptor`.
- **[LOW] `partition_id` defaults documented.** Vertex Functions table left the default
  column blank. Code: `partition_id(g,u)` defaults to `0` (partition 0); there is **no**
  `partition_id(g,uid)` default in the reference impl. Filled `0 (partition 0)` for the
  descriptor form and added a note that the id form is not yet implemented.
- **[NOTE] `vertex_value(g,uid)` complexity left as "constant"** — consistent with the
  other id-form convenience overloads in the same table under the stated index-graph
  assumption (find_vertex is O(1)); changing only this row would create an inconsistency.

PDF rebuilds cleanly (0 undefined refs). Build note: the `lstset` uses `texcl=true`, so
`//` comments inside `lstlisting`/`lstinputlisting` are rendered as LaTeX — underscores
in such comments must be escaped (`\_`).

---

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

### 1.9 [LOW] `mapped_vertex_range` missing `hashable_vertex_id` — RESOLVED (documented in paper)
- Original gap: the paper's `mapped_vertex_range` snippet omitted the
  `hashable_vertex_id<G>` requirement present in the code, and the
  `hashable_vertex_id` concept was not mentioned at all.
- Resolution (paper updated): `src/concepts_vertex_range.hpp` now defines
  `hashable_vertex_id<G>` and includes it in `mapped_vertex_range`; the Vertex
  Concepts prose introduces the concept (vertex id usable as an `unordered_map`
  key, used by `mapped_vertex_range` and the `vertex_property_map` utilities) and
  updates the `mapped_vertex_range` bullet to mention it. PDF rebuilds cleanly.

### 1.10 [LOW] `vertex<G,V>` missing `is_vertex_descriptor_v` guard — RESOLVED (added to snippet)
- Original gap: the paper's `vertex<G,V>` snippet omitted the
  `is_vertex_descriptor_v<remove_cvref_t<V>>` guard present in the code.
- Resolution (paper updated): the `vertex` concept snippet now leads with the
  descriptor guard, and the Vertex Concepts prose explains it distinguishes a
  vertex descriptor from a raw vertex value so the concept does not accidentally
  match unrelated types. PDF rebuilds cleanly.

---

## 2. Clarity / completeness observations

### 2.1 [LOW] Partition default-implementation semantics — RESOLVED (paper clarified)
- Original gap: the Graph Functions table gave the `vertices(g,pid)` /
  `num_vertices(g,pid)` defaults as `vertices(g)` / `size(vertices(g))` without
  the `pid==0` condition the code applies (empty range / 0 otherwise).
- Resolution (paper updated): the table now reads `vertices(g)` if `pid==0`
  else empty range, and `size(vertices(g))` if `pid==0` else 0. PDF rebuilds
  cleanly.

### 2.2 [LOW] Undocumented `num_edges(g,u)` overload — RESOLVED (documented as alias)
- Original gap: the code has per-vertex `num_edges(g,u)` / `num_edges(g,uid)`
  CPOs (same result as `out_degree`) that the paper did not document.
- Resolution (paper updated): added an alias line to the Vertex Functions table
  noting `num_edges(g,u), num_edges(g,uid)` are aliases for `out_degree`,
  alongside the existing `edges`/`degree` alias rows. PDF rebuilds cleanly.

### 2.3 Concept count wording — RESOLVED
- Paper: "Six concepts are defined" for the adjacency list — accurate for the
  documented set. Code additionally defines `ordered_vertex_edges` (used by
  triangle-count in the algorithms paper) and `hashable_vertex_id` (used by
  `mapped_vertex_range`). `ordered_vertex_edges` is left to the algorithms paper;
  `hashable_vertex_id` is now documented in this paper (see 1.9).

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
- **Q5 (`hashable_vertex_id`):** RESOLVED — documented in this paper; it is
  GCI-local and underpins `mapped_vertex_range` and the `vertex_property_map`
  utilities. See 1.9.
