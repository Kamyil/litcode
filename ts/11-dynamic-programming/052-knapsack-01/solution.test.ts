import { expect, test } from "bun:test";
import { canPartition, knapsack } from "./solution";

test("knapsack", () => {
  expect(knapsack([1, 3, 4, 5], [1, 4, 5, 7], 7)).toBe(9);
  expect(knapsack([10, 20, 30], [60, 100, 120], 50)).toBe(220);
  expect(knapsack([5], [10], 4)).toBe(0);
  expect(knapsack([2, 2, 2], [3, 3, 3], 4)).toBe(6); // not 9: each item once
});

test("canPartition", () => {
  expect(canPartition([1, 5, 11, 5])).toBe(true);
  expect(canPartition([1, 2, 3, 5])).toBe(false);
  expect(canPartition([2, 2])).toBe(true);
  expect(canPartition([1])).toBe(false);
  expect(canPartition(Array(200).fill(100))).toBe(true);
});
