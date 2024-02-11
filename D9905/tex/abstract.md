# Graph Library: Graph Container Interface
The the core interface used for uniformly accessing graph data structures by views and algorithms.

It is also designed to easily adapt to existing graph data structures. More information can also be found in the *Graph Containers* paper.

This is one of a number of papers related to the propsed Graph Libray, shown in the following table.
It replaces the *Graph Container Interface* section of the previous [P1709r5](http:://wg21.link/P1709r5) paper.


| Paper                           | Number | Status                                                                          | 
| :-------------------------------| :----- | :-------------------------------------------------------------------------------|
| Overview                        |        | Describes the big picture of what we are proposing, covering the scope and our initial focus.         |
| Background and Terminology      |        | Provides the motivation, theoretical background and terminology used across the other documents.      |
| Algorithms                      |        | Describes the initial graph algorithms as well as the ones we'd like to see in the future.            |
| Views                           |        | Includes helpful views for traversing a graph, such as `vertexlist(g)` and `vertices_bfs(g,uid)`.     |
| **Graph Container Interface**   |        | **The core interface used for uniformly accessing graph data structures by views and algorithms. It is also designed to easily adapt to existing graph data structures.** |
| Graph Containers                |        | Describes a proposed high-performance `compressed_graph` container. It also discusses how to adapt other graph data structures to be used by the views and algorithms. |

Additional papers for *Graph Operators* and *Graph Adaptors* are expected in the future.

All papers include a *Getting Started* section to help the reader get oriented.
