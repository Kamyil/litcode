// Binary tree node used by the tree problems.
export class TreeNode {
  constructor(
    public val: number,
    public left: TreeNode | null = null,
    public right: TreeNode | null = null,
  ) {}
}

// Builds a tree from LeetCode-style level order: [3, 9, 20, null, null, 15, 7]
export function treeFrom(values: (number | null)[]): TreeNode | null {
  if (values.length === 0 || values[0] === null) return null;
  const root = new TreeNode(values[0]);
  const queue: TreeNode[] = [root];
  let i = 1;
  while (queue.length && i < values.length) {
    const node = queue.shift()!;
    const l = values[i++];
    if (l !== null && l !== undefined) queue.push((node.left = new TreeNode(l)));
    const r = values[i++];
    if (r !== null && r !== undefined) queue.push((node.right = new TreeNode(r)));
  }
  return root;
}

// Inverse of treeFrom (trailing nulls trimmed).
export function treeToArray(root: TreeNode | null): (number | null)[] {
  const out: (number | null)[] = [];
  const queue: (TreeNode | null)[] = [root];
  while (queue.length) {
    const node = queue.shift()!;
    if (node) {
      out.push(node.val);
      queue.push(node.left, node.right);
    } else out.push(null);
  }
  while (out.length && out[out.length - 1] === null) out.pop();
  return out;
}
