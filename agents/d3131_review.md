# D3131 Containers — Review Comments

> **Status (2026-06-09):** All findings below have been APPLIED to the paper and
> both PDFs rebuild cleanly. The D3130/D3131 scope split (Q1) was resolved:
> D3130 keeps the normative recognition *patterns*; D3131 received the concrete
> container *catalog*, trade-off table, and worked examples. The reconcile
> question **1.2** (requires-clause) and scope questions **Q2–Q5** are resolved.
> Per-item status is tagged inline as *(APPLIED)*.

Review of `D3131_Containers/tex/containers.tex` (plus snippets
`src/compressed_graph.hpp`, `src/compressed_graph_gvoid.hpp` and `revision.tex`)
against the reference implementation in
`/home/phil/dev_graph/graph-v3/include/graph/container/compressed_graph.hpp`.

Scope per request: only `compressed_graph` is in scope as a concrete container
(the code's `dynamic_graph` and `undirected_adjacency_list` are correctly
omitted). Adjacency lists and edgelists built from standard containers are in
scope and must be thoroughly described with examples. Topics owned by the GCI
paper (D3130) — concepts, CPO defaults, descriptors, the recognized vertex/edge
patterns — are referenced but not re-specified here.

Legend: **[HIGH]** = code/paper genuinely disagree on the interface; **[MED]** =
documented symbol/behavior absent or different in the reference impl, or an
internal contradiction; **[LOW]** = wording/snippet polish.

---

## 1. Code-vs-paper inconsistencies

### 1.1 [HIGH] `edge_info` is a stale name — should be `edge_data` *(APPLIED)*
- Paper (Edgelist std-container example): `using EL = vector<edge_info<int, true, void, double>>;`
  and prose "use the `edge_info` used in this proposal." The `gvoid` snippet also
  has the comment "initializer list using `edge_info<VId,true,void,EV>`".
- Code: there is no `edge_info` anywhere in `include/`. The aggregate is
  `graph::edge_data<VId, Sourced, E, EV>` (graph_data.hpp), and the
  initializer-list ctor takes `copyable_edge_t<VId, EV>` (=`edge_data<VId,true,void,EV>`).
- Fix: replace `edge_info` with `edge_data` (and use `copyable_edge_t<VId,EV>`
  where the ctor's element type is meant). This matches D3130, which already uses
  `edge_data`/`vertex_data`/`neighbor_data`. Confirmed by author: `edge_info` was
  renamed to `edge_data`.

### 1.2 [HIGH] `requires copyable_edge/copyable_vertex` removed in paper but still in code *(APPLIED — code is source of truth)*
- `revision.tex` r3 states the `copyable_vertex`/`copyable_edge` constraints were
  "removed from the `compressed_graph` requires clause because descriptors are
  always copyable," and the `compressed_graph.hpp` snippet shows the value
  ctors constrained only by `requires convertible_to<range_value_t<PartRng>, VId>`.
- Code: every value/edge ctor still carries
  `requires copyable_edge<invoke_result_t<EProj, range_value_t<ERng>>, VId, EV>`
  (and `copyable_vertex<...>` for the vrng overloads) in addition to the
  `convertible_to<...,VId>` clause (compressed_graph.hpp lines ~1500–1582, 1626–1652).
- **Resolution (author-confirmed):** the constraint is *kept*; the r3 note was a
  misreading. `copyable_edge<T,VId,EV>` is `convertible_to<T, copyable_edge_t<VId,EV>>`
  (graph_data.hpp) — it validates that the **`EProj`/`VProj` projection result** is
  convertible to the `{source_id,target_id,value}` / `{id,value}` aggregate the ctor
  expects. It is a type check on the projected value, unrelated to descriptor
  copyability, so descriptor copyability does not make it redundant.
- **Applied:** the full-`GV` `compressed_graph.hpp` snippet now carries the
  `copyable_edge`/`copyable_vertex` requires clauses on all six value ctors (matching
  the code and the `gvoid` snippet); the r3 revision bullet was rewritten from
  "Remove … because descriptors are always copyable" to "Retain … to validate the
  projection result type." The `\mandates` prose (already fixed in 1.4) is consistent.

### 1.3 [HIGH] `has_edge(g)` should be `has_edges(g)` *(APPLIED)*
- Paper summary box: `has_edge(g)` is $O(1)$; `revision.tex` r1 likewise lists
  `has_edge(g)`.
- Code/D3130: the CPO is `has_edges(g)` (plural) — `graph_cpo.hpp` defines
  `inline constexpr ... has_edges{}`; D3130's Graph Functions table documents
  `has_edges(g)`. There is no `has_edge` CPO.
- Fix: rename to `has_edges(g)` in the summary box and revision history.

### 1.4 [MED] `copyable_edge<VId, true, EV>` is wrong arity in `\mandates` *(APPLIED)*
- Paper `\mandates`: "EProj … must return a value of `copyable_edge<VId, true, EV>`
  type"; "VProj … must return a value of `copyable_vertex<VId, VV>` type."
- Code: the concept is `copyable_edge<T, VId, EV>` (3 params; `T` is the projected
  value), and the *type alias* the projection must produce is
  `copyable_edge_t<VId, EV>` (= `edge_data<VId,true,void,EV>`). There is no
  `true` template argument on `copyable_edge`. Likewise `copyable_vertex_t<VId,VV>`.
- Fix: phrase as "must return a `copyable_edge_t<VId, EV>` (resp.
  `copyable_vertex_t<VId, VV>`)," or reference the concept as
  `copyable_edge<projected-value, VId, EV>`. Drop the stray `true`.

### 1.5 [MED] Edgelist namespace is `edge_list`, not `edgelist` *(APPLIED)*
- Paper: "Type alias are in the namespace `std::graph::edgelist` to avoid
  conflicts with adjacency_list types."
- Code/D3130: the namespace is `graph::edge_list` (underscore) — see
  `edge_list/edge_list.hpp` (`namespace edge_list`). `edgelist` (no underscore)
  is only the *view* (`views::edgelist`), a different thing.
- Fix: `std::graph::edge_list`.

### 1.6 [MED] Edgelist examples use 1-arg CPOs; the convention is 2-arg `f(el, uv)` *(APPLIED)*
- Paper edgelist examples:
  `int sid = source_id(e); int tid = target_id(e); double val = edge_value(e);`
- Code/D3130: edgelist CPOs use the 2-argument form `source_id(el, uv)`,
  `target_id(el, uv)`, `edge_value(el, uv)` (mirroring the adjacency-list
  convention; "All edgelist CPOs require both the container and the element for
  container-specific dispatch," D3130 Edgelist Concepts).
- Fix: `source_id(el, e)`, `target_id(el, e)`, `edge_value(el, e)` in both
  edgelist examples.

### 1.7 [MED] `vertices(g,u,pid)` in the override table should be `vertices(g,pid)` *(APPLIED)*
- Paper Table (Common CPO Function Overrides), partitions group: lists
  `vertices(g,u,pid)`.
- Code/D3130: the partition-filtered overload is `vertices(g, pid)` (2-arg) — the
  `compressed_graph` friend is `vertices(G&& g, const PId& pid)`. There is no
  `vertices(g,u,pid)`.
- Fix: `vertices(g,pid)`.

### 1.8 [MED] `partition_id(g,uid)` — code only provides the descriptor form *(APPLIED)*
- Paper summary box: `partition_id(g,uid)` is $O(\log(P+1))$.
- Code: the friend/CPO is `partition_id(g, u)` taking a **vertex descriptor**
  (compressed_graph.hpp line 1383), complexity $O(\log P)$ via
  `upper_bound(partition_.begin(), partition_.end()-1, vid)`. D3130 already notes
  the `partition_id(g,uid)` id-form is not yet implemented.
- Fix: use `partition_id(g,u)`. The $\log(P{+}1)$ vs $\log P$ footnote is
  defensible (the `end()-1` search range is P elements); align the wording to the
  code if you want exactness.

---

## 2. Internal contradictions / correctness

### 2.1 [MED] Partition preconditions: "undefined behavior" vs code throws *(APPLIED)*
- Paper `\preconditions`: "the first element must be 0 and all elements are in
  ascending order… A vertex id … must not exceed the number of vertices… Any
  violation of these conditions results in undefined behavior."
- Code (`terminate_partitions`, lines 896–921): violations are **defined** —
  `throw graph_error(...)` when `partition_[0] != 0`, when not strictly
  increasing (`partition_[i] <= partition_[i-1]`), or when
  `partition_.back() > row_index_.size()`.
- Fix: replace "undefined behavior" with "throws `graph_error`," and state
  "strictly increasing" rather than "ascending."

### 2.2 [MED] `\phil` note on duplicate partition ids contradicts the code *(APPLIED — note removed)*
- Paper `\phil{If duplicate partition_start_ids exist they create an empty
  partition with no vertices.}`
- Code: duplicates are rejected — `partition_[i] <= partition_[i-1]` throws
  `graph_error` ("must be in strictly increasing order"). Duplicates do **not**
  create empty partitions.
- Fix: drop/revise the note; empty partitions would require a different (gap)
  encoding than the current strict-increasing rule allows.

### 2.3 [MED] `edge_value(g,uv)` row in `tab:simple_graph` contradicts the prose *(APPLIED)*
- Paper prose (std-container adjacency example): "The `edge_value` function is
  **not** defined, as it is assumed that algorithms will take a lambda to extract
  the value."
- Same section, Table `tab:simple_graph`: row `edge_value(g,uv)` →
  `tuple<int,double>`.
- Also, per D3130 the default `edge_value`/`inner_value` for a 2-tuple
  `tuple<int,double>` returns `get<1>` (the `double`), not the whole tuple.
- Fix: either remove the `edge_value(g,uv)` row (consistent with "not defined"),
  or correct its type. As written it is both self-contradictory and not what the
  default CPO yields.

### 2.4 [LOW] `tab:simple_graph` rows `u` and `uv` reflect the pre-descriptor model *(APPLIED)*
- Paper: `u` → `forward_list<tuple<int,double>>`, `uv` → `tuple<int,double>`.
- Under the r3 boost-like descriptor design (D3130), `vertices(g)` yields a
  `vertex_descriptor` (not the inner range) and `edges(g,u)` yields
  `edge_descriptor`s; the raw `forward_list`/`tuple` are the *underlying* values
  reached via the descriptor, not `u`/`uv` themselves.
- Fix: relabel as the underlying/inner value (e.g. "underlying value of `u`"),
  or note these are the underlying container elements, to stay consistent with
  the descriptor abstraction used elsewhere in this proposal.

---

## 3. Snippet fidelity (`src/*.hpp` exposition listings)

### 3.1 [MED] `compressed_graph_gvoid.hpp` has a malformed class header *(APPLIED)*
- Snippet:
  ```
  template <class EV, class VV, integral VId=uint32_t, integral EIndex=uint32_t,
            class Alloc=allocator<VId>>
  template <class EV, class VV, integral VId, integral EIndex, class Alloc>
  class compressed_graph<EV, VV, void, VId, EIndex, Alloc>
  public: // Construction/Destruction
  ```
  Two stacked `template` headers, no `: public compressed_graph_base<...>`, and a
  missing `{` before `public:`.
- Code: a single specialization header
  `template <class EV, class VV, integral VId, integral EIndex, class Alloc>
   class compressed_graph<EV, VV, void, VId, EIndex, Alloc>
        : public compressed_graph_base<EV, VV, void, VId, EIndex, Alloc> {`.
- Fix: drop the first (defaulted) `template` line, add the base-class clause and
  the opening brace so the exposition compiles in a reader's head.

### 3.2 [LOW] `gvoid` ctor prototypes drop the trailing `;` *(APPLIED)*
- The edge-only and edge+vertex ctor prototypes in `compressed_graph_gvoid.hpp`
  end with `)` and no `;` (unlike the full `compressed_graph.hpp` listing, whose
  prototypes are terminated with `;`). Add `;` for consistency.

### 3.3 [LOW] `gvoid` init-list comment uses the stale `edge_info` *(APPLIED)*
- Comment "initializer list using `edge_info<VId,true,void,EV>`." Code comment
  reads `edge_data<VId,true,void,EV>`. Update to `edge_data` /
  `copyable_edge_t<VId,EV>` (see 1.1).

### 3.4 [LOW] Adjacency example code has small slips *(APPLIED)*
- `auto weight = [&g](edge_t& uv) { return get<1>(uv); }` — missing `;`, `edge_t`
  should be `edge_t<G>`, and the lambda captures `&g` before `g` is declared
  (declare `G g;` first, or capture nothing and pass the graph). Also
  `for(auto&& [u] : vertices(g))` decomposes a descriptor as a 1-tuple; with the
  descriptor model this should be `for(auto&& u : vertices(g))` (or iterate the
  `vertexlist` view to get `[uid, u]`). Tighten so the example compiles.

---

## 4. Confirmed consistent (spot-checked, no action)
- Full-`GV` constructor set in `compressed_graph.hpp` snippet matches the code:
  `(alloc)`, `(gv&, alloc)`, `(gv&&, alloc)`; the erng+eproj trio; the
  erng+vrng+eproj+vproj trio; and the `initializer_list<copyable_edge_t<VId,EV>>`
  ctor — all present with the same shapes and `partition_start_ids` defaults.
- Template parameter order/defaults `<EV, VV, GV, VId=uint32_t, EIndex=uint32_t,
  Alloc=allocator<VId>>` match the code's primary template.
- `compressed_graph` is CSR, immutable structure after construction, values on
  vertex/edge/graph mutable — matches (`csr_row_values`/`csr_col_values`,
  `graph_value()` accessor; no public add/remove vertex/edge).
- `void` EV/VV/GV → no storage overhead: matches the empty specializations of
  `csr_row_values`/`csr_col_values` and the `GV=void` class specialization.
- Summary box: vertex_id contiguous, contiguous vertices/edges ranges,
  `num_edges(g)` $O(1)$, `has_edges(g)` $O(1)$, `num_partitions` $O(1)$,
  Append vertices/edges = No, Partitions = Yes — all consistent with the code.
- `is_directed` not supported for `compressed_graph` (no specialization in
  `compressed_graph.hpp`); undirected use requires inserting both `(u,v)` and
  `(v,u)` — matches.
- Memory-size formula
  $|V|(sizeof(EIndex)+sizeof(VV)) + |E|(sizeof(VId)+sizeof(EV)) + sizeof(GV)$ is
  consistent with the CSR layout (`row_index_`+row values sized to |V|, `col_index_`
  + col values sized to |E|).
- Std-container adjacency patterns `random_access_range<forward_range<integral>>`
  and `random_access_range<forward_range<tuple<integral,...>>>` with the integral
  as target_id — matches D3130's recognized patterns. The "no overrides required"
  claim is correct.

---

## 5. Open questions (scope / intent — need author input)

- **Q1 (duplication with D3130):** D3130 ("Using Existing Data Structures") and
  D3131 ("Using Existing Data Structures") both describe std-container adjacency
  lists and edgelists. `revision.tex` r0 says this text was *moved from* the GCI
  to this paper, yet D3130 retains an extensive "Recognized Vertex/Edge Patterns"
  treatment. Which paper owns the canonical description, and how much should be
  cross-referenced vs. duplicated?
  - **RESOLVED & APPLIED:** D3130 keeps the *normative recognition patterns* (the
    structural trigger: random-access / associative vertex patterns and the
    integral/`pair`/`tuple` edge element patterns) and points to D3131 for the
    catalog. D3131 received the concrete container example lists, the
    `tab:edge_container_patterns` trade-off table, and the combined examples.
    Both revision histories updated.

- **Q2 (`operator[]`):** `\phil{Add operator[](vertex_id_t<G>)?}` — the internal
  `csr_row_values`/`csr_col_values` expose `operator[]`, but the public
  `compressed_graph` does not. Is a public `operator[]` intended for this paper,
  or deferred? (Recommend resolving the `\phil` note before publication.)
  - **RESOLVED & APPLIED — not added.** `operator[]` would only expose an internal
    CSR element that isn't useful on its own, and it would breach the
    CPO-only public surface. The `\phil` note was replaced with a short rationale
    pointing to `vertex_value(g,uid)` and `out_edges(g,*find_vertex(g,uid))` as the
    generic equivalents.

- **Q3 (movable EV/VV):** `\phil{Is it possible to support movable EV and VV
  types?}` — `\mandates` currently requires `EV`/`VV` to be *copyable*. Is this a
  hard requirement to keep, or a future relaxation to note?
  - **RESOLVED & APPLIED — copyable kept; movable noted as future work.** The
    `\phil` note was replaced with a rationale: move-only `EV`/`VV` would disable
    the `initializer_list` ctor (const elements), restrict construction to the
    rvalue-range overloads, make the graph move-only (deleted copy ctor), and need
    a GCI-level relaxation of `copyable_edge`/`copyable_vertex`. `GV` already may be
    movable.

- **Q4 (more examples):** The request asks that std-container adjacency lists be
  *thoroughly* described with examples. Currently there are two adjacency
  examples (`vector<forward_list<tuple<int,double>>>` and
  `vector<forward_list<int>>`) and two edgelist examples. Should an
  associative/mapped example (e.g. `map<int, vector<pair<int,double>>>`) be added
  here, or is that owned by D3130's pattern tables? (Ties to Q1.)
  - **RESOLVED & APPLIED — added here.** A worked `map<int, vector<pair<int,double>>>`
    associative example now follows the catalog's associative bullet list: same GCI
    loop shape, vertex id is the map key (`vertex_id(g,u)`), `pair.first` is the
    target id, `pair.second` is the edge value. A note explains it models
    `mapped_adjacency_list` but not `index_adjacency_list` (so `find_vertex` is
    `O(log|V|)` / amortized `O(1)`), with the normative concept details left to D3130.

- **Q5 (undirected mutable-edge example):** `\phil{Add example(s) … mutable edge
  values for an undirected graph with duplicate (u,v)/(v,u) edges.}` — open TODO;
  confirm whether it belongs in this paper.
  - **RESOLVED & APPLIED — note removed.** Per author: undirected graphs need more
    attention but a different approach will be used, so the placeholder example is
    not pursued here. The `\phil` TODO was removed from the paper.
