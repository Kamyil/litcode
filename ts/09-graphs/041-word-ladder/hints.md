Put `wordList` in a `Set`. BFS from `begin`, level by level, counting levels.
---
Neighbors: for each position, each letter `a..z`, build the candidate; if it's in the set,
delete it from the set (marks visited) and enqueue.
