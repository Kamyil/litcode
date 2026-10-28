import { expect, test } from "bun:test";
import { TreeNode, treeFrom, treeToArray } from "../../lib/tree";
import { deserialize, serialize } from "./solution";

const roundTrip = (t: TreeNode | null) => treeToArray(deserialize(serialize(t)));

test("round trips", () => {
  for (const arr of [[1, 2, 3, null, null, 4, 5], [], [1], [-1, 0, 1], [1, null, 2, null, 3], [100, -200, 300]]) {
    expect(roundTrip(treeFrom(arr))).toEqual(arr);
  }
});

test("output is a string", () => {
  expect(typeof serialize(treeFrom([1, 2]))).toBe("string");
});

test("deep tree (10k)", () => {
  let root: TreeNode | null = null;
  for (let i = 0; i < 10_000; i++) root = new TreeNode(i, null, root);
  let n = deserialize(serialize(root));
  let depth = 0;
  while (n) {
    depth++;
    n = n.right;
  }
  expect(depth).toBe(10_000);
});
