# 028 · Max Depth & Diameter of a Binary Tree

> **Difficulty:** Easy · **Topics:** tree DFS, recursion returning values · **Target:** O(n)

- `maxDepth(root)`: number of nodes on the longest root-to-leaf path.
- `diameter(root)`: number of **edges** on the longest path between any two nodes (the path
  doesn't have to pass through the root).
- `maxDepthIterative(root)`: max depth without recursion (explicit stack or BFS queue).

`TreeNode`, `treeFrom([3,9,20,null,null,15,7])` live in `ts/lib/tree.ts`.

## Think about

- Diameter is the template for a huge class of tree problems: the recursive function returns
  one thing (height) to its parent but updates a *global* answer with another (left + right).
- A degenerate tree (a linked list) of 100k nodes overflows the JS call stack with recursion.
  That's why the iterative version exists. Postgres has `check_stack_depth()` sprinkled
  through recursive code for the same reason.
