import { expect, test } from "bun:test";
import { longestConsecutive } from "./solution";

test("examples", () => {
  expect(longestConsecutive([100, 4, 200, 1, 3, 2])).toBe(4);
  expect(longestConsecutive([0, 3, 7, 2, 5, 8, 4, 6, 0, 1])).toBe(9);
  expect(longestConsecutive([])).toBe(0);
});

test("large", () => {
  const nums = Array.from({ length: 300_000 }, (_, i) => (i * 7919) % 300_000);
  expect(longestConsecutive(nums)).toBe(300_000);
});
