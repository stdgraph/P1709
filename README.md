# C++ Graph Library Proposal for the Standard C++ Library

Ths repository contains the documents (in latex form) for a proposed standard graph library. 
The documents are as follows:

| Paper                           | Number | Status | Description                                                                          | 
| :-------------------------------| :----- | :----- |:-------------------------------------------------------------------------------|
| Original Proposal               | P1709  | Retired | Split into the following papers.                                              |
| Overview                        | [P3126](http:://wg21.link/P3126)  | Active | Describes the big picture of what we are proposing, covering the scope and our initial focus.         |
| Background and Terminology      | [P3127](http:://wg21.link/P3127)  | Active | Provides the motivation, theoretical background and terminology used across the other documents.      |
| Algorithms                      | [P3128](http:://wg21.link/P3128)  | Active | Describes the initial graph algorithms as well as the ones we'd like to see in the future.            |
| Views                           | [P3129](http:://wg21.link/P3129)  | Active | Includes helpful views for traversing a graph, such as `vertexlist(g)` and `vertices_bfs(g,uid)`.     |
| Graph Container Interface       | [P3130](http:://wg21.link/P3130)  | Active | The core interface used for uniformly accessing graph data structures by views and algorithms. It is also designed to easily adapt to existing graph data structures. |
| Graph Containers                | [P3131](http:://wg21.link/P3131)  | Active | Describes a proposed high-performance `compressed_graph` container. It also discusses how to adapt other graph data structures to be used by the views and algorithms. |
| Operators                       | D9903 | Future |                        |
| Adaptors                        | D9907 | Future |                         |
| boost::graph Comparison         |       | Future | Compares syntax and performance.      |

Future papers are assigned temporary draft numbers until they are published. They are only relevant within this project.

## Building

It is assumed you have a recent version of some TeX distribution installed.

A Makefile has build rules for all of the documents.

```
% make
```
will build them all.  Intermediate files are kept in the aux subdirectory.  Final pdf files are kept in the pdf subdirectory.


Individual pdfs can be built thusly:
```
% make D3128.pdf
```

The intermediate files can all be cleaned up:
```
% make clean
```

## Related
The reference implementation can be found at https://github.com/stdgraph/graph-v2.
