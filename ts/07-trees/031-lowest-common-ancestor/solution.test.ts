import { expect, test } from "bun:test";
import { treeFrom } from "../../lib/tree";
import { lca, lcaBST } from "./solution";

test("BST", () => {
  const t = treeFrom([6, 2, 8, 0, 4, 7, 9, null, null, 3, 5]);
  expect(lcaBST(t, 2, 8)).toBe(6);
  expect(lcaBST(t, 2, 4)).toBe(2);
  expect(lcaBST(t, 3, 5)).toBe(4);
  expect(lcaBST(t, 0, 5)).toBe(2);
  expect(lcaBST(t, 7, 7)).toBe(7);
});

test("general tree", () => {
  const t = treeFrom([3, 5, 1, 6, 2, 0, 8, null, null, 7, 4]);
  expect(lca(t, 5, 1)).toBe(3);
  expect(lca(t, 5, 4)).toBe(5);
  expect(lca(t, 7, 8)).toBe(3);
  expect(lca(t, 6, 4)).toBe(5);
  expect(lca(treeFrom([1, 2]), 1, 2)).toBe(1);
});
