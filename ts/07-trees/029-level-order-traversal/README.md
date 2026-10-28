# 029 · Binary Tree Level Order Traversal

> **Difficulty:** Medium · **Topics:** BFS, queue · **Target:** O(n)

- `levelOrder(root)`: values level by level, left to right: `[[3], [9, 20], [15, 7]]`.
- `zigzagLevelOrder(root)`: same but alternate direction each level: `[[3], [20, 9], [15, 7]]`.
- `rightSideView(root)`: the value you'd see from the right on each level: `[3, 20, 7]`.

## Think about

- The "process one level at a time" trick: snapshot `queue.length` at the start of each level.
- `Array.prototype.shift()` is O(n) in the worst case in many engines. For big inputs use a
  head index into the array instead. (A ring buffer — sys/105 — is the real-world answer.)
