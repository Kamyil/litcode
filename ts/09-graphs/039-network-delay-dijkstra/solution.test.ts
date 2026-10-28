import { expect, test } from "bun:test";
import { networkDelayTime, shortestPath } from "./solution";

test("networkDelayTime", () => {
  expect(networkDelayTime([[2, 1, 1], [2, 3, 1], [3, 4, 1]], 4, 2)).toBe(2);
  expect(networkDelayTime([[1, 2, 1]], 2, 1)).toBe(1);
  expect(networkDelayTime([[1, 2, 1]], 2, 2)).toBe(-1);
  expect(networkDelayTime([[1, 2, 5], [1, 3, 1], [3, 2, 1]], 3, 1)).toBe(2);
});

test("shortestPath", () => {
  const t: [number, number, number][] = [[1, 2, 7], [1, 3, 9], [1, 6, 14], [2, 3, 10], [2, 4, 15], [3, 4, 11], [3, 6, 2], [4, 5, 6], [6, 5, 9]];
  expect(shortestPath(6, t, 1, 5)).toEqual([1, 3, 6, 5]);
  expect(shortestPath(6, t, 1, 1)).toEqual([1]);
  expect(shortestPath(6, t, 5, 1)).toEqual([]);
});

test("dense-ish graph performance", () => {
  const n = 2000;
  const t: [number, number, number][] = [];
  for (let u = 1; u <= n; u++) for (let j = 1; j <= 20; j++) t.push([u, ((u + j * 37) % n) + 1, (u * j) % 17]);
  expect(networkDelayTime(t, n, 1)).toBeGreaterThanOrEqual(0);
});
