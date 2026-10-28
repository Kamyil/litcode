import { TreeNode } from "../../lib/tree";

export function maxDepth(root: TreeNode | null): number {
  return root ? 1 + Math.max(maxDepth(root.left), maxDepth(root.right)) : 0;
}

export function diameter(root: TreeNode | null): number {
  let best = 0;
  const height = (n: TreeNode | null): number => {
    if (!n) return 0;
    const hl = height(n.left);
    const hr = height(n.right);
    best = Math.max(best, hl + hr);
    return 1 + Math.max(hl, hr);
  };
  height(root);
  return best;
}

export function maxDepthIterative(root: TreeNode | null): number {
  const stack: [TreeNode, number][] = root ? [[root, 1]] : [];
  let best = 0;
  while (stack.length) {
    const [n, d] = stack.pop()!;
    best = Math.max(best, d);
    if (n.left) stack.push([n.left, d + 1]);
    if (n.right) stack.push([n.right, d + 1]);
  }
  return best;
}
