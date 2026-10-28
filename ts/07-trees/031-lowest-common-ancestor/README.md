# 031 · Lowest Common Ancestor

> **Difficulty:** Medium · **Topics:** tree recursion · **Target:** O(n) (O(h) for a BST)

The LCA of nodes `p` and `q` is the deepest node that has both as descendants (a node is a
descendant of itself). Both `p` and `q` exist in the tree; values are unique.

- `lcaBST(root, p, q)`: the tree is a BST — use that. O(h), no recursion needed.
- `lca(root, p, q)`: a general binary tree.

Inputs are **values**; return the LCA's value.

## Examples

```
BST [6,2,8,0,4,7,9,null,null,3,5]:   lcaBST(2, 8) -> 6,  lcaBST(2, 4) -> 2
Tree [3,5,1,6,2,0,8,null,null,7,4]:  lca(5, 1) -> 3,     lca(5, 4) -> 5
```
