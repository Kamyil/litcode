import { expect, test } from "bun:test";
import { maxArea } from "./solution";

test("examples", () => {
  expect(maxArea([1, 8, 6, 2, 5, 4, 8, 3, 7])).toBe(49);
  expect(maxArea([1, 1])).toBe(1);
  expect(maxArea([4, 3, 2, 1, 4])).toBe(16);
  expect(maxArea([1, 2, 1])).toBe(2);
});

test("large input", () => {
  const h = Array.from({ length: 200_000 }, (_, i) => (i * 31) % 10_000);
  expect(maxArea(h)).toBeGreaterThan(0);
});
