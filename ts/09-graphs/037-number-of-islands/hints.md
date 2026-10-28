Scan every cell; when you hit unvisited land, count++ and flood-fill the whole island marking it visited.
---
Flood fill iteratively: a stack of cell indices; pop, check 4 neighbors, push unvisited land.
