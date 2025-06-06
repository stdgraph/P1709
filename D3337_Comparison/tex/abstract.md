# Graph Library: Algorithms
Covers the initial graph algorithms as well as the ones we'd like to see in the future.

This is one of a number of papers related to the propsed Graph Libray, shown in the following table.
It replaces the *Algorithms* section of the previous [P1709r5](http:://wg21.link/P1709r5) paper.

| Paper                           | Number | Status                                                                          | 
| :-------------------------------| :----- | :-------------------------------------------------------------------------------|
| Overview                        | [P3126](http:://wg21.link/P3126)  | Describes the big picture of what we are proposing, covering the scope and our initial focus.         |
| Background and Terminology      | [P3127](http:://wg21.link/P3127)  | Provides the motivation, theoretical background and terminology used across the other documents.      |
| **Algorithms**                  | [P3128](http:://wg21.link/P3128)  | **Describes the initial graph algorithms as well as the ones we'd like to see in the future.**        |
| Views                           | [P3129](http:://wg21.link/P3129)  | Includes helpful views for traversing a graph, such as `vertexlist(g)` and `vertices_bfs(g,uid)`.     |
| Graph Container Interface       | [P3130](http:://wg21.link/P3130)  | The core interface used for uniformly accessing graph data structures by views and algorithms. It is also designed to easily adapt to existing graph data structures. |
| Graph Containers                | [P3131](http:://wg21.link/P3131)  | Describes a proposed high-performance `compressed_graph` container. It also discusses how to adapt other graph data structures to be used by the views and algorithms. |
| Graph Library Comparison        | [P3337](http:://wg21.link/P3337)  | Provides a performance and usage syntax comparison of proposed graph library to existing graph libraries. |

Additional papers for *Graph Operators* and *Graph Adaptors* are expected in the future.

All papers include a *Getting Started* section to help the reader get oriented.
