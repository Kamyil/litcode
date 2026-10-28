import { expect, test } from "bun:test";
import { climbStairs, minCostClimbingStairs } from "./solution";

test("climbStairs", () => {
  expect([1, 2, 3, 4, 5].map(climbStairs)).toEqual([1, 2, 3, 5, 8]);
  expect(climbStairs(45)).toBe(1836311903);
  expect(climbStairs(70)).toBe(308061521170129);
});

test("minCostClimbingStairs", () => {
  expect(minCostClimbingStairs([10, 15, 20])).toBe(15);
  expect(minCostClimbingStairs([1, 100, 1, 1, 1, 100, 1, 1, 100, 1])).toBe(6);
});
