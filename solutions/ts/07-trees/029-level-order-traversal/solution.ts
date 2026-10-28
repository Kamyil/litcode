import { TreeNode } from "../../lib/tree";

export function levelOrder(root: TreeNode | null): number[][] {
  const out: number[][] = [];
  let level: TreeNode[] = root ? [root] : [];
  while (level.length) {
    out.push(level.map((n) => n.val));
    const next: TreeNode[] = [];
    for (const n of level) {
      if (n.left) next.push(n.left);
      if (n.right) next.push(n.right);
    }
    level = next;
  }
  return out;
}

export function zigzagLevelOrder(root: TreeNode | null): number[][] {
  return levelOrder(root).map((lvl, i) => (i % 2 ? lvl.reverse() : lvl));
}

export function rightSideView(root: TreeNode | null): number[] {
  return levelOrder(root).map((lvl) => lvl[lvl.length - 1]);
}
