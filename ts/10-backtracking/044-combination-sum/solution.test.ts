import { expect, test } from "bun:test";
import { combinationSum, combinationSum2 } from "./solution";

const norm = (xs: number[][]) => xs.map((s) => [...s].sort((a, b) => a - b).join(",")).sort();

test("combinationSum", () => {
  expect(norm(combinationSum([2, 3, 6, 7], 7))).toEqual(norm([[2, 2, 3], [7]]));
  expect(norm(combinationSum([2, 3, 5], 8))).toEqual(norm([[2, 2, 2, 2], [2, 3, 3], [3, 5]]));
  expect(combinationSum([2], 1)).toEqual([]);
});

test("combinationSum2", () => {
  expect(norm(combinationSum2([10, 1, 2, 7, 6, 1, 5], 8))).toEqual(norm([[1, 1, 6], [1, 2, 5], [1, 7], [2, 6]]));
  expect(norm(combinationSum2([2, 5, 2, 1, 2], 5))).toEqual(norm([[1, 2, 2], [5]]));
  expect(combinationSum2(Array(30).fill(1), 30)).toEqual([Array(30).fill(1)]);
});
