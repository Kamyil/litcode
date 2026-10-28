import { expect, test } from "bun:test";
import { KthLargest, findKthLargest } from "./solution";

test("findKthLargest", () => {
  expect(findKthLargest([3, 2, 1, 5, 6, 4], 2)).toBe(5);
  expect(findKthLargest([3, 2, 3, 1, 2, 4, 5, 5, 6], 4)).toBe(4);
  expect(findKthLargest([1], 1)).toBe(1);
});

test("KthLargest stream", () => {
  const s = new KthLargest(3, [4, 5, 8, 2]);
  expect([3, 5, 10, 9, 4].map((v) => s.add(v))).toEqual([4, 5, 5, 8, 8]);
  const e = new KthLargest(1, []);
  expect(e.add(-3)).toBe(-3);
  expect(e.add(-2)).toBe(-2);
});
