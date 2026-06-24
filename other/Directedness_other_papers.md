# Applicability of `other/Directedness.htm` to Other Papers

This file records where Andrew Lumsdaine's directedness email thread
(see `other/Directedness_D3127.md` for the full analysis) has implications
beyond D3127 (Terminology).

---

## D3130 — Graph Container Interface (strong relevance)

The GCI already defines six adjacency-list concepts with no directed/undirected
axis — the correct outcome. However, the introduction still describes the adjacency
list as a "graph" (`"(aka graphs)"` in `container_interface.tex` §1), conflating the
two. Andrew's sharper framing — *adjacency list ≠ graph; an adjacency list carries
no directedness* — should inform how the GCI describes what its concepts model.

Additionally, the `sorted_adjacency_list` concept is directly analogous to the
`is_sorted` / `is_symmetric` precondition argument Andrew makes: sortedness is a
value-level property enforced by precondition, not a type. The paper could draw that
explicit analogy to justify the design.

**Recommended change:** Revise the introductory description to say the GCI defines
the interface for *adjacency lists* (and edge lists), not "graphs," and note that
directedness is a property of which edges the caller stores, not a type parameter.

---

## D3131 — Containers (direct framing conflict)



## D3128 — Algorithms (moderate relevance)

Algorithm summary boxes use `Directed? Yes / No` as a static property, and text
says algorithms operate on "directed/undirected graphs." Andrew's position is that
algorithms take *"an adjacency list of a directed/undirected graph"* rather than a
directed/undirected type — and that `triangle_count`'s undirected requirement is a
*precondition* (`is_symmetric`) not a type constraint.

**Recommended changes:**
- In algorithm descriptions, prefer "requires a symmetric adjacency list" (or
  "requires the adjacency list of an undirected graph") over "operates on an
  undirected graph."
- Consider adding an `is_symmetric` precondition note to `triangle_count` and any
  other algorithm whose correctness depends on symmetric adjacencies.
- The commented-out text at lines 131/133 (adjacency list as range-of-ranges) is
  correct and could be restored/updated with the clarification that the structure
  itself carries no directedness.

---

## D3129 — Views (minor relevance)

The `incidence` / `in_incidence` view split corresponds directly to Andrew's
*incidence list* vs. *bidirectional incidence list* distinction. His point — that
these out/in half-edges are *representation-level* notions and still do not enforce
directedness — could appear as a brief note in the views introduction.

**Recommended change:** Add a sentence near the `incidence` / `in_incidence`
introduction noting that the presence of `in_incidence` support reflects the
underlying graph's structure but does not itself guarantee symmetry; that remains a
value-level property.

---

## D3337 — Comparison (informational / citation opportunity)

D3337 already correctly notes (line 21) that `std::graph` does not specify edge
direction as a graph property and that undirected graphs store edges in both
directions — which is precisely the behavior the email justifies. The paper frames
this as a difference from BGL's `undirectedS` without explaining the design
rationale.

**Recommended change:** Add a forward reference (or brief sentence) pointing to D3127
for the principled justification: directedness cannot be enforced by the type system,
so the design intentionally leaves it as a value-level precondition rather than a
type tag.
