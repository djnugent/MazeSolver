# Maze Solver

Created this Maze solver to learn how to implement a graph in C++ and implement path planning(BFS,DFS,A*,D*,etc)

## Build
1. make `build` directory and enter it
2. run `cmake -G "Visual Studio Win64" ..` 
3. compile with `cmake --build .`
4. Generate a maze using `gen.py`
5. Executable can be found in `build/Debug/`

## Design Notes
- Files aren't linking properly so implementation is included in headers
- `Edge` and `Node` classes are co-dependent so they use forward declaration to solve compiler issues
- The graph store the nodes. All access/modifications must use pointers and references to update the graph in place
- Edges store node pointers

## Status
- BFS: Traversal with early ending
- DFS: Traversal with early ending
- A*: In queue
- Dikjstra: In Queue
- D*: In Queue
- D* lite: In Queue