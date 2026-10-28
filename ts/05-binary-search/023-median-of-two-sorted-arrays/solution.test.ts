import { expect, test } from "bun:test";
import { findMedianSortedArrays } from "./solution";

const naive = (a: number[], b: number[]) => {
  const all = [...a, ...b].sort((x, y) => x - y);
  const n = all.length;
  return n % 2 ? all[(n - 1) / 2] : (all[n / 2 - 1] + all[n / 2]) / 2;
};

test("examples", () => {
  expect(findMedianSortedArrays([1, 3], [2])).toBe(2);
  expect(findMedianSortedArrays([1, 2], [3, 4])).toBe(2.5);
  expect(findMedianSortedArrays([], [1])).toBe(1);
  expect(findMedianSortedArrays([2], [])).toBe(2);
});

test("random cross-check", () => {
  let seed = 42;
  const rnd = (n: number) => (seed = (seed * 16807) % 2147483647) % n;
  for (let t = 0; t < 500; t++) {
    const a = Array.from({ length: rnd(8) }, () => rnd(20)).sort((x, y) => x - y);
    const b = Array.from({ length: rnd(8) + (a.length ? 0 : 1) }, () => rnd(20)).sort((x, y) => x - y);
    expect(findMedianSortedArrays(a, b)).toBe(naive(a, b));
  }
});
