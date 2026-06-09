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

### 1.2 [HIGH] `edge_descriptor::edge_storage_type` differs from the implementation
- Paper: `src/descriptor.hpp` shows
  `edge_storage_type = conditional_t<random_access_iterator<EdgeIter>, size_t, EdgeIter>`,
  and the surrounding prose ("Index-based: one integer") implies an index is
  stored for random-access edge containers.
- Code: `adj_list/edge_descriptor.hpp` — `using edge_storage_type = EdgeIter;`
  (always the iterator; no index specialization). The `vertex_descriptor`
  *does* use the `size_t`/iterator conditional, but the `edge_descriptor` does
  not.
- Action: fix the `edge_descriptor` snippet to store `EdgeIter`, and adjust the
  copy-size discussion so the index claim applies to `vertex_descriptor` only.

### 1.3 [MED] Edgelist `vertex_id_t<EL>`: paper says `target_id`, code uses `source_id`
- Paper: Table "Edgelist Interface Type Aliases" defines
  `vertex_id_t<EL> = remove_cvref_t<decltype(target_id(el,uv))>`, and the
  Namespace section says the edgelist `vertex_id_t` "is derived ... from
  `target_id(el,uv)`."
- Code: `edge_list/edge_list.hpp` derives both `vertex_id_t` and
  `raw_vertex_id_t` from **`source_id(el,uv)`**.
- The two ids are constrained to be the same type, so this is harmless at
  runtime, but the documentation should match the implementation (or vice
  versa). Pick one consistently.

### 1.4 [MED] `vertex_id_store_t<G>` does not exist
- Paper: Table "Graph Container Interface Type Aliases" lists
  `vertex_id_store_t<G>` and labels it "normative".
- Code: no such alias. The closest is `raw_vertex_id_t<G>` (the paper's
  "*raw-vertex-id-type*", exposition only).
- Action: remove `vertex_id_store_t<G>` or replace with the actual alias name
  and definition.

### 1.5 [MED] Adjacency-matrix trait family is undocumented-as-future and partly absent
- Paper: traits table lists `define_adjacency_matrix` (flagged "Not yet in
  reference implementation"), plus `is_adjacency_matrix`, `is_adjacency_matrix_v`,
  and the `adjacency_matrix` concept (no flag). The `contains_out_edge` default
  in Table "Edge Functions" describes a constant-time path guarded by
  `is_adjacency_matrix_v<G>`.
- Code: none of `define_adjacency_matrix`, `is_adjacency_matrix`,
  `is_adjacency_matrix_v`, or `adjacency_matrix` exist. The actual
  `contains_edge`/`contains_out_edge` default always does a linear `find_if` —
  there is no adjacency-matrix O(1) shortcut.
- Action: either (a) mark the whole family + the `contains_out_edge` constant
  branch as "not yet in reference implementation", or (b) drop them. See Open
  Question Q1.

### 1.6 [MED] Partition-filtered `out_edges(g,*,pid)` not implemented
- Paper: the partitions section states `out_edges(g,uid,pid)` and
  `out_edges(g,u,pid)` "filter the edges where the target is in the partition
  `pid`."
- Code: no `out_edges` overload takes a `pid`. Only `vertices(g,pid)`,
  `num_vertices(g,pid)`, `num_partitions(g)`, and `partition_id(g,u)` exist.
- Action: mark as future, or remove. See Open Question Q2.

### 1.7 [MED] Edgelist functions `num_edges/has_edges/contains_edge` are TODO in code
- Paper: Table "Edgelist Interface Functions" lists `num_edges(el)`,
  `has_edges(el)`, and `contains_edge(el,uid,vid)` with default implementations.
- Code: `edge_list/edge_list.hpp` header marks these "(todo)"; only
  `source_id`, `target_id`, `edge_value` CPOs are implemented in `edge_cpo.hpp`.
- Action: confirm intent — keep documented as the target interface, or add a
  "not yet implemented" note. See Open Question Q3.

### 1.8 [LOW] `vertex_value(g,uid)` convenience overload missing in code
- Paper: vertex-functions table lists `vertex_value(g,uid)` with default
  `vertex_value(g,*find_vertex(g,uid))`.
- Code: `_vertex_value` CPO provides only `operator()(g, u)` (descriptor form);
  no `uid` overload.
- Action: either add the overload to the impl or footnote it as not-yet-provided.

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

- **Q1 (adjacency matrix):** Should the adjacency-matrix trait family
  (`define_adjacency_matrix`, `is_adjacency_matrix`, `is_adjacency_matrix_v`,
  `adjacency_matrix`) and the constant-time `contains_out_edge` branch stay in
  this paper as *future design*, or be removed until implemented? (See 1.5.)
- **Q2 (partition edge filtering):** Keep `out_edges(g,uid,pid)` /
  `out_edges(g,u,pid)` as documented future API, or remove? (See 1.6.)
- **Q3 (edgelist functions):** Are `num_edges(el)` / `has_edges(el)` /
  `contains_edge(el,uid,vid)` part of the proposed interface (document as-is) or
  should they be flagged "not yet implemented"? (See 1.7.)
- **Q4 (edgelist `vertex_id`):** Should the edgelist `vertex_id_t` be derived
  from `source_id` (as the code does) or `target_id` (as the paper says)? (See
  1.3.)
- **Q5 (`hashable_vertex_id`):** Does the `hashable_vertex_id` concept belong in
  this paper (it is GCI-local and underpins `mapped_vertex_range`), or is it
  intentionally hidden? (See 1.9.)
