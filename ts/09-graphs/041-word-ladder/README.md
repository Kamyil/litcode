# 041 · Word Ladder

> **Difficulty:** Hard · **Topics:** BFS shortest path, implicit graph · **Target:** O(N · L · 26)

Transform `begin` into `end` by changing one letter at a time; every intermediate word must
be in `wordList`. Return the number of words in the shortest sequence (including both ends),
or `0` if impossible.

```
begin = "hit", end = "cog", wordList = ["hot","dot","dog","lot","log","cog"]
"hit" -> "hot" -> "dot" -> "dog" -> "cog"   ->  5
```

## Think about

- BFS on an unweighted graph gives shortest paths. The graph is **implicit** — don't build
  all N² edges. Generate neighbors by trying all 26 letters at each position, or bucket by
  wildcard patterns (`h*t`).
- Bidirectional BFS (from both ends, always expanding the smaller frontier) is a big speedup
  and a great follow-up.
