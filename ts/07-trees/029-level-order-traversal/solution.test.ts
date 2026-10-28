import { expect, test } from "bun:test";
import { treeFrom } from "../../lib/tree";
import { levelOrder, rightSideView, zigzagLevelOrder } from "./solution";

const t = treeFrom([3, 9, 20, null, null, 15, 7]);

test("levelOrder", () => {
  expect(levelOrder(t)).toEqual([[3], [9, 20], [15, 7]]);
  expect(levelOrder(null)).toEqual([]);
  expect(levelOrder(treeFrom([1, 2, 3, 4, 5, 6, 7]))).toEqual([[1], [2, 3], [4, 5, 6, 7]]);
});

test("zigzag", () => {
  expect(zigzagLevelOrder(t)).toEqual([[3], [20, 9], [15, 7]]);
  expect(zigzagLevelOrder(treeFrom([1, 2, 3, 4, 5, 6, 7]))).toEqual([[1], [3, 2], [4, 5, 6, 7]]);
});

test("rightSideView", () => {
  expect(rightSideView(treeFrom([1, 2, 3, null, 5, null, 4]))).toEqual([1, 3, 4]);
  expect(rightSideView(treeFrom([1, 2, 3, 4]))).toEqual([1, 3, 4]);
  expect(rightSideView(null)).toEqual([]);
});
