import { expect, test } from "bun:test";
import { twoSum } from "./solution";

test("examples", () => {
  expect(twoSum([2, 7, 11, 15], 9)).toEqual([0, 1]);
  expect(twoSum([3, 2, 4], 6)).toEqual([1, 2]);
  expect(twoSum([3, 3], 6)).toEqual([0, 1]);
});

test("negatives", () => {
  expect(twoSum([-3, 4, 3, 90], 0)).toEqual([0, 2]);
});

test("large input runs in linear time", () => {
  const n = 200_000;
  const nums = Array.from({ length: n }, (_, i) => i * 2);
  expect(twoSum(nums, (n - 2) * 2 + (n - 1) * 2)).toEqual([n - 2, n - 1]);
});
