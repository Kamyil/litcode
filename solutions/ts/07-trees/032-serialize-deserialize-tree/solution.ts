import { TreeNode } from "../../lib/tree";

// Level order with "#" for null children: iterative in both directions.
export function serialize(root: TreeNode | null): string {
  const out: string[] = [];
  const queue: (TreeNode | null)[] = [root];
  for (let i = 0; i < queue.length; i++) {
    const n = queue[i];
    if (n) {
      out.push(String(n.val));
      queue.push(n.left, n.right);
    } else out.push("#");
  }
  return out.join(",");
}

export function deserialize(data: string): TreeNode | null {
  const tokens = data.split(",");
  if (tokens[0] === "#") return null;
  const root = new TreeNode(Number(tokens[0]));
  const queue: TreeNode[] = [root];
  let t = 1;
  for (let i = 0; i < queue.length; i++) {
    const n = queue[i];
    const l = tokens[t++];
    const r = tokens[t++];
    if (l !== "#") queue.push((n.left = new TreeNode(Number(l))));
    if (r !== "#") queue.push((n.right = new TreeNode(Number(r))));
  }
  return root;
}
