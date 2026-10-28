# 037 · Number of Islands

> **Difficulty:** Medium · **Topics:** grid DFS/BFS, flood fill · **Target:** O(rows · cols)

Grid of `"1"` (land) and `"0"` (water). Count islands: groups of land connected
horizontally or vertically.

Then `maxAreaOfIsland(grid)`: the size (cell count) of the biggest island.

Don't mutate the caller's grid (the tests check) — copy it or keep a `visited` structure.

## Examples

```
11110
11010     ->  1 island
11000
00000

11000
11000     ->  3 islands
00100
00011
```

## Think about

- A grid is an implicit graph: cell = vertex, neighbors = edges. Recursion depth can reach
  rows·cols on a snake-shaped island — iterative BFS/DFS is safer (test includes a 300x300 all-land grid).
- `Uint8Array(rows * cols)` with index `r * cols + c` is a much cheaper `visited` than a `Set<string>`.
