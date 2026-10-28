import { expect, test } from "bun:test";
import { rob, robCircle } from "./solution";

test("rob", () => {
  expect(rob([1, 2, 3, 1])).toBe(4);
  expect(rob([2, 7, 9, 3, 1])).toBe(12);
  expect(rob([2, 1, 1, 2])).toBe(4);
  expect(rob([])).toBe(0);
  expect(rob([5])).toBe(5);
});

test("robCircle", () => {
  expect(robCircle([2, 3, 2])).toBe(3);
  expect(robCircle([1, 2, 3, 1])).toBe(4);
  expect(robCircle([1, 2, 3])).toBe(3);
  expect(robCircle([7])).toBe(7);
});
