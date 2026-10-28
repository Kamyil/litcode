import { expect, test } from "bun:test";
import { canFinish, findOrder } from "./solution";

function isValidOrder(n: number, prereqs: [number, number][], order: number[]): boolean {
  if (order.length !== n || new Set(order).size !== n) return false;
  const pos = new Map(order.map((c, i) => [c, i]));
  return prereqs.every(([a, b]) => pos.get(b)! < pos.get(a)!);
}

test("canFinish", () => {
  expect(canFinish(2, [[1, 0]])).toBe(true);
  expect(canFinish(2, [[1, 0], [0, 1]])).toBe(false);
  expect(canFinish(3, [])).toBe(true);
  expect(canFinish(4, [[1, 0], [2, 1], [3, 2], [1, 3]])).toBe(false);
  expect(canFinish(1, [[0, 0]])).toBe(false);
});

test("findOrder", () => {
  const p: [number, number][] = [[1, 0], [2, 0], [3, 1], [3, 2]];
  expect(isValidOrder(4, p, findOrder(4, p))).toBe(true);
  expect(findOrder(2, [[0, 1], [1, 0]])).toEqual([]);
  expect(findOrder(1, [])).toEqual([0]);
});

test("long chain", () => {
  const n = 50_000;
  const p = Array.from({ length: n - 1 }, (_, i) => [i + 1, i] as [number, number]);
  expect(isValidOrder(n, p, findOrder(n, p))).toBe(true);
});
