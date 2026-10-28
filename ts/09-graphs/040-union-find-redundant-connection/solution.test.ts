import { expect, test } from "bun:test";
import { UnionFind, countComponents, findRedundantConnection } from "./solution";

test("UnionFind", () => {
  const uf = new UnionFind(5);
  expect(uf.count).toBe(5);
  expect(uf.union(0, 1)).toBe(true);
  expect(uf.union(1, 2)).toBe(true);
  expect(uf.union(0, 2)).toBe(false);
  expect(uf.connected(0, 2)).toBe(true);
  expect(uf.connected(0, 3)).toBe(false);
  expect(uf.count).toBe(3);
});

test("countComponents", () => {
  expect(countComponents(5, [[0, 1], [1, 2], [3, 4]])).toBe(2);
  expect(countComponents(5, [[0, 1], [1, 2], [2, 3], [3, 4]])).toBe(1);
  expect(countComponents(3, [])).toBe(3);
});

test("findRedundantConnection", () => {
  expect(findRedundantConnection([[1, 2], [1, 3], [2, 3]])).toEqual([2, 3]);
  expect(findRedundantConnection([[1, 2], [2, 3], [3, 4], [1, 4], [1, 5]])).toEqual([1, 4]);
});

test("a million unions stays fast", () => {
  const n = 1_000_000;
  const uf = new UnionFind(n);
  for (let i = 1; i < n; i++) uf.union(i - 1, i);
  expect(uf.count).toBe(1);
  expect(uf.connected(0, n - 1)).toBe(true);
});
