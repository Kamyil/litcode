import { TreeNode } from "../../lib/tree";

export function lcaBST(root: TreeNode | null, p: number, q: number): number {
  let n = root;
  while (n) {
    if (p < n.val && q < n.val) n = n.left;
    else if (p > n.val && q > n.val) n = n.right;
    else return n.val;
  }
  throw new Error("not found");
}

export function lca(root: TreeNode | null, p: number, q: number): number {
  const go = (n: TreeNode | null): TreeNode | null => {
    if (!n || n.val === p || n.val === q) return n;
    const l = go(n.left);
    const r = go(n.right);
    return l && r ? n : (l ?? r);
  };
  return go(root)!.val;
}
