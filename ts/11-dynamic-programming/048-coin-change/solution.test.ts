import { expect, test } from "bun:test";
import { coinChange, coinChangeWays } from "./solution";

test("coinChange", () => {
  expect(coinChange([1, 2, 5], 11)).toBe(3);
  expect(coinChange([2], 3)).toBe(-1);
  expect(coinChange([1], 0)).toBe(0);
  expect(coinChange([1, 3, 4], 6)).toBe(2); // greedy says 3 (4+1+1)
  expect(coinChange([186, 419, 83, 408], 6249)).toBe(20);
});

test("coinChangeWays", () => {
  expect(coinChangeWays([1, 2, 5], 5)).toBe(4);
  expect(coinChangeWays([2], 3)).toBe(0);
  expect(coinChangeWays([10], 10)).toBe(1);
  expect(coinChangeWays([1, 2, 5], 0)).toBe(1);
});
