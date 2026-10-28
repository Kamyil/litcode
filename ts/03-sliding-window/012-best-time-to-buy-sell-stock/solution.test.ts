import { expect, test } from "bun:test";
import { maxProfit, maxProfitUnlimited } from "./solution";

test("single transaction", () => {
  expect(maxProfit([7, 1, 5, 3, 6, 4])).toBe(5);
  expect(maxProfit([7, 6, 4, 3, 1])).toBe(0);
  expect(maxProfit([2, 4, 1])).toBe(2);
  expect(maxProfit([])).toBe(0);
});

test("unlimited transactions", () => {
  expect(maxProfitUnlimited([7, 1, 5, 3, 6, 4])).toBe(7);
  expect(maxProfitUnlimited([1, 2, 3, 4, 5])).toBe(4);
  expect(maxProfitUnlimited([7, 6, 4, 3, 1])).toBe(0);
});
