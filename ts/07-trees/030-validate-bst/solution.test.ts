import { expect, test } from "bun:test";
import { treeFrom } from "../../lib/tree";
import { isValidBST, kthSmallest } from "./solution";

test("isValidBST", () => {
  expect(isValidBST(treeFrom([2, 1, 3]))).toBe(true);
  expect(isValidBST(treeFrom([5, 1, 4, null, null, 3, 6]))).toBe(false);
  expect(isValidBST(treeFrom([5, 4, 6, null, null, 3, 7]))).toBe(false);
  expect(isValidBST(treeFrom([2, 2, 2]))).toBe(false);
  expect(isValidBST(null)).toBe(true);
  expect(isValidBST(treeFrom([-2147483648, null, 2147483647]))).toBe(true);
});

test("kthSmallest", () => {
  const t = treeFrom([5, 3, 6, 2, 4, null, null, 1]);
  expect([1, 2, 3, 4, 5, 6].map((k) => kthSmallest(t, k))).toEqual([1, 2, 3, 4, 5, 6]);
  expect(kthSmallest(treeFrom([3, 1, 4, null, 2]), 1)).toBe(1);
});
