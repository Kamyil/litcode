BST: if both values are smaller than `node.val` go left; both bigger, go right; otherwise you're at the split = LCA.
---
General: `lca(node)` returns the node if it is p or q; otherwise recurse both sides. If both sides
return non-null, this node is the LCA; else pass up whichever side is non-null.
