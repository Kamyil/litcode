import { expect, test } from "bun:test";
import { solveNQueens, totalNQueens } from "./solution";

test("solveNQueens", () => {
  expect(solveNQueens(4).map((b) => b.join("|")).sort()).toEqual(
    [[".Q..", "...Q", "Q...", "..Q."], ["..Q.", "Q...", "...Q", ".Q.."]].map((b) => b.join("|")).sort(),
  );
  expect(solveNQueens(1)).toEqual([["Q"]]);
  expect(solveNQueens(3)).toEqual([]);
  expect(solveNQueens(8).length).toBe(92);
});

test("totalNQueens", () => {
  expect([1, 2, 3, 4, 5, 6, 7, 8].map(totalNQueens)).toEqual([1, 0, 0, 2, 10, 4, 40, 92]);
  expect(totalNQueens(12)).toBe(14200);
});
