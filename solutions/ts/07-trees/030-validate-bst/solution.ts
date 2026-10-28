import { TreeNode } from "../../lib/tree";

export function isValidBST(root: TreeNode | null): boolean {
  const valid = (n: TreeNode | null, lo: number, hi: number): boolean =>
    !n || (lo < n.val && n.val < hi && valid(n.left, lo, n.val) && valid(n.right, n.val, hi));
  return valid(root, -Infinity, Infinity);
}

export function kthSmallest(root: TreeNode | null, k: number): number {
  const stack: TreeNode[] = [];
  let n = root;
  while (n || stack.length) {
    while (n) {
      stack.push(n);
      n = n.left;
    }
    n = stack.pop()!;
    if (--k === 0) return n.val;
    n = n.right;
  }
  throw new Error("k out of range");
}
