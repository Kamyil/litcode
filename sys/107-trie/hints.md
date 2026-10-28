Validate the whole word before inserting anything, so a bad character doesn't leave half a path.
---
`prefix_count` on each node = number of words passing through (or ending at) it. Increment along
the path on a *new* insert, decrement on a successful remove. `count_prefix` = the counter at the prefix node.
---
Remove: first check `contains` (so you don't decrement counters for a missing word). Then walk down
decrementing; any child whose count hits 0 can be freed together with its whole subtree — and that
subtree is just a chain at that point.
---
collect: recursive DFS with a char buffer `buf[depth] = 'a' + i`, `buf[depth+1] = '\0'` at word ends.
A word can be at most as long as the longest inserted word — track `t->max_len`, or grow a heap buffer.
---
Rust remove: the same counter trick lets you `node.children[i] = None` the moment a child's count hits 0 —
dropping the Box frees the rest.
