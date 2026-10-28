import { expect, test } from "bun:test";
import { largestRectangleArea } from "./solution";

test("examples", () => {
  expect(largestRectangleArea([2, 1, 5, 6, 2, 3])).toBe(10);
  expect(largestRectangleArea([2, 4])).toBe(4);
});

test("edge cases", () => {
  expect(largestRectangleArea([])).toBe(0);
  expect(largestRectangleArea([0])).toBe(0);
  expect(largestRectangleArea([3, 3, 3])).toBe(9);
  expect(largestRectangleArea([1, 2, 3, 4, 5])).toBe(9);
  expect(largestRectangleArea([2, 1, 2])).toBe(3);
});
