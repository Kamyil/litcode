import { expect, test } from "bun:test";
import { TreeNode, treeFrom } from "../../lib/tree";
import { diameter, maxDepth, maxDepthIterative } from "./solution";

test("maxDepth", () => {
  expect(maxDepth(treeFrom([3, 9, 20, null, null, 15, 7]))).toBe(3);
  expect(maxDepth(treeFrom([1, null, 2]))).toBe(2);
  expect(maxDepth(null)).toBe(0);
});

test("diameter", () => {
  expect(diameter(treeFrom([1, 2, 3, 4, 5]))).toBe(3);
  expect(diameter(treeFrom([1, 2]))).toBe(1);
  expect(diameter(treeFrom([1]))).toBe(0);
  // longest path does not go through the root
  expect(diameter(treeFrom([1, 2, null, 3, 4, 5, null, null, 6, 7, null, null, 8]))).toBe(6);
});

test("iterative survives a 200k-deep tree", () => {
  let root: TreeNode | null = null;
  for (let i = 0; i < 200_000; i++) root = new TreeNode(i, root);
  expect(maxDepthIterative(root)).toBe(200_000);
  expect(maxDepthIterative(treeFrom([3, 9, 20, null, null, 15, 7]))).toBe(3);
  expect(maxDepthIterative(null)).toBe(0);
});
