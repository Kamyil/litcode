# 030 · Validate Binary Search Tree (+ kth smallest)

> **Difficulty:** Medium · **Topics:** BST invariant, in-order traversal · **Target:** O(n)

- `isValidBST(root)`: every node's left subtree holds only **strictly smaller** keys and its
  right subtree only **strictly larger** keys.
- `kthSmallest(root, k)`: 1-based k-th smallest key in a valid BST.

## Think about

- The classic bug: checking only `left.val < node.val < right.val`. Tree `[5,4,6,null,null,3,7]`
  passes that check but `3` sits in the right subtree of `5`. Pass down `(min, max)` bounds.
- In-order traversal of a BST yields sorted keys. That's the whole reason B-tree indexes can
  serve `ORDER BY` and range queries without sorting.
- kthSmallest follow-up: if the tree is modified often and you query kth often, what would
  you store in each node? (subtree sizes — an "order statistic tree")
