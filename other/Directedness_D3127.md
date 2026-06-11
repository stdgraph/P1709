# Review of `other/Directedness.htm` — Recommendations for D3127 (Terminology)

## Source

`other/Directedness.htm` is an email thread on **"P1709: directedness?"** It begins with
Phil Ratzloff's question — *"What does directed, or undirected, mean?"* (with Kevin Deweese's
suggestion that the distinction is "there's a matching edge in the opposite direction") — and is
answered at length by Andrew Lumsdaine. Andrew's central thesis is that **directedness is a
property of values, not of types or data structures**, and therefore the terminology document
must be precise about what carries directedness and what does not.

Below, each of Andrew's points is mapped to a concrete recommendation for
[D3127_Terminology/tex/terminology_0.tex](D3127_Terminology/tex/terminology_0.tex).

---

## Key claims in the email

1. Algorithms generally take an **adjacency list**, not a "graph". An adjacency list is *not* a
   graph; it is a sparse form of the adjacency matrix that records *whether two vertices are
   adjacent*.
2. An adjacency list is **neither directed nor undirected** — Andrew notes CLRS states this
   explicitly. It just represents adjacency and has no actual edges; to traverse "the other
   direction" you need a transpose.
3. An **edge list**, by contrast, *does* contain edges, and those edges *are* directed or
   undirected.
4. The literature **conflates adjacency list with graph** (e.g. "Dijkstra takes a graph"). More
   precise: an algorithm takes *an adjacency list from a directed graph* or *from an undirected
   graph*.
5. (3a) An adjacency list of an undirected graph is **indistinguishable** from the adjacency list
   of a directed graph that happens to have a reciprocal edge `(v,u)` for every `(u,v)` — but not
   vice versa.
6. Directedness / symmetry **cannot be enforced by the type system**. You can declare an adjacency
   list "comes from an undirected graph" yet build it asymmetrically. This is exactly analogous to
   a symmetric matrix in linear algebra, or to `sortedness` in the standard library: binary search
   has a *precondition* `is_sorted`, not a type. So there should be an `is_symmetric`-style
   predicate, **not** a directed/undirected *type* and **not** a directedness *concept*.
7. **Incidence list / incidence graph** (BGL): `out_edges`, plus `in_edges` for a bidirectional
   incidence graph. These half-edges look directed but are subject to the same
   non-enforceability — they are neither directed nor undirected even when the underlying graph is.
8. Recommended terminology: **"adjacency list of a graph"** and **"incidence list of a graph"**.
9. **Edge properties**: one can "fake" an incidence list from an adjacency list by attaching
   properties (e.g. `vector<vector<tuple<int, float>>>`). This needs terminology — perhaps a
   **"half-edge"** concept and a **"symmetric property map"** (again a value-level, not type-level,
   constraint).

### Concepts Andrew proposes
- *adjacency list* — supports `adjacent_vertices`
- *incidence list* — supports `out_edges`
- *bidirectional incidence list* — additionally supports `in_edges`
- *readable / writeable property map* — supports `get` / `put`

---

## Recommendations for D3127

### 1. State explicitly that an adjacency list is neither directed nor undirected
This is the email's headline point and is currently **only present as a commented-out `\andrew{}`
note** near the end of the source ("the structure of an adjacency list does not capture
directedness -- directedness is a run-time property"). The body text defines directed/undirected
for *graphs* and *edges*, and shows that for an undirected graph `J[i]` contains `j` and `J[j]`
contains `i`, but never states the converse conclusion.

**Add** (in the "Adjacency-Based Representations" subsection, after the compressed sparse adjacency
matrix is introduced): a sentence making explicit that the adjacency list / compressed sparse
adjacency matrix itself carries *no* directedness — directedness is a property of which entries are
present (a value/run-time property), not of the data structure. Cite CLRS for the claim that an
adjacency list is neither directed nor undirected to strengthen it.

### 2. Add a dedicated subsection: "Directedness is a property of values, not types"
The doc currently has no place that answers Phil's original question head-on. **Add** a short
subsection (or a clearly-flagged paragraph) covering:
- Directed vs. undirected is a property of the **edge set's contents** (whether reciprocal pairs
  exist), not of any representation type.
- Therefore it **cannot be enforced by the type system** and should **not** be modeled as a
  separate directed/undirected type or as a concept.
- Draw the explicit analogies Andrew gives: a *symmetric matrix* in linear algebra, and
  *sortedness* (`is_sorted` precondition) in the standard library; suggest an `is_symmetric`-style
  predicate as the corresponding check.

This directly resolves Kevin's "matching edge in the opposite direction" framing: an undirected
graph's representation is one whose stored adjacencies are symmetric.

### 3. Adopt "adjacency list of a graph" / "incidence list of a graph" phrasing
Reflect Andrew's point #4/#8. Where the doc says algorithms operate on graphs, prefer the precise
"an adjacency list *of* (or *from*) a directed/undirected graph." The doc already makes the
graph-vs-representation distinction strongly; this extends that discipline to how directedness is
attributed.

### 4. Make the indistinguishability point explicit (email 3a)
The coordinate-representation paragraph already observes there is "not a 1-1 correspondence between
the edges in `E` and the contents of `C`" for undirected graphs. **Extend** this with Andrew's
sharper statement: the adjacency list of an undirected graph is *indistinguishable* from that of a
directed graph containing a reciprocal edge for every edge — but not vice versa. This reinforces
why directedness cannot be recovered from the representation alone.

### 5. Define "incidence list" (and bidirectional incidence list) — not just incidence *matrix*
The doc has an "Incident Matrices" subsection (note: heading reads "Incident", body reads
"Incidence" — make consistent), but no **incidence list** terminology, despite the email and the
algorithms relying on `out_edges` / `in_edges`. **Add** terminology for:
- *incidence list of a graph* — exposes out-edges;
- *bidirectional incidence list* — additionally exposes in-edges;
and clarify that these "half-edges" / out/in distinctions are still not enforceable directedness —
they mirror the underlying graph but the structure itself does not guarantee it.

> Note: this interacts with the existing bullet in "Basic Terminology" that, for the *undirected*
> case, says *"The edge `e_k` is an out-edge of both `v_i` and `v_j` and it is an in-edge of both."*
> Per the email's framing, in/out are representation-level notions on an incidence list rather than
> properties of an undirected edge. Reconcile this wording so the in/out vocabulary is introduced
> with the incidence representation, not with undirected edges in the abstract graph.

### 6. Add terminology for edge properties on adjacency lists ("half-edge", "symmetric property map")
Points #8–#10 are not addressed anywhere in D3127. **Add** terminology to cover the common
`vector<vector<tuple<index, property>>>` pattern (adjacency list carrying per-adjacency property
data), introduce the **half-edge** notion, and note the **symmetric property map** requirement
(that the property of `(u,v)` equal that of `(v,u)`) as another *value-level* precondition — not a
type/concept. A brief forward-reference to where concepts/property maps are formally defined would
suffice if full treatment belongs elsewhere.

### 7. Cite CLRS for "adjacency list is neither directed nor undirected"
The doc already adopts CLRS as its terminology baseline. Andrew explicitly attributes the
"neither directed nor undirected" claim to Cormen et al.; adding that citation gives the
recommendation in #1 authoritative backing.

---

## Adjacent observations (surfaced while cross-checking the directedness claims)

These are quality issues noticed in the directedness-related passages; worth fixing while editing
the same sections:

- **Directed vs. undirected adjacency-matrix definitions are identical in the LaTeX.** In the
  "Adjacency-Based Representations" subsection both `a_{ij}` cases use `(v_i, v_j) \in E`; the
  undirected one should express symmetry (`a_{ij} = a_{ji}`) via an unordered edge `\{v_i, v_j\}`.
  The following sentence also has a typo: *"the difference ... is that and the adjacency matrix"*
  and `a_ij` is missing braces (`$a_{ij}$`).
- **"path" is defined twice and inconsistently** (once in Basic Terminology allowing the simple
  sequence, once in special cases requiring distinct vertices), and the **"cycle"** definition
  ("every vertex appears twice") is garbled. A self-loop is written `${v_i, v_i}$` (missing
  `\{ \}`). These are not directedness issues but sit in the same terminology section.
