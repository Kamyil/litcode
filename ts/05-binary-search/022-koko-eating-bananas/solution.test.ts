import { expect, test } from "bun:test";
import { minEatingSpeed } from "./solution";

test("examples", () => {
  expect(minEatingSpeed([3, 6, 7, 11], 8)).toBe(4);
  expect(minEatingSpeed([30, 11, 23, 4, 20], 5)).toBe(30);
  expect(minEatingSpeed([30, 11, 23, 4, 20], 6)).toBe(23);
});

test("huge piles need log time", () => {
  expect(minEatingSpeed([1_000_000_000], 2)).toBe(500_000_000);
  expect(minEatingSpeed([1_000_000_000, 1_000_000_000], 3)).toBe(1_000_000_000);
});
