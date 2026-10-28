import { expect, test } from "bun:test";
import { trap } from "./solution";

test("examples", () => {
  expect(trap([0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1])).toBe(6);
  expect(trap([4, 2, 0, 3, 2, 5])).toBe(9);
});

test("edge cases", () => {
  expect(trap([])).toBe(0);
  expect(trap([5])).toBe(0);
  expect(trap([1, 2, 3, 4])).toBe(0);
  expect(trap([5, 0, 5])).toBe(5);
});
