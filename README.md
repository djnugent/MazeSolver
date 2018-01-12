# Maze Solver

## Build
1. make `build` directory and enter it
2. run `cmake -G "Visual Studio Win64" ..` 
3. compile with `cmake --build .`

## Design notes
- Files aren't linking properly so implemtation is included in headers
- `Edge` and `Node` classes are co-dependent so they use forward declaration to solve compiler issues