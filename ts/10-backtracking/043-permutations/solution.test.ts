import { expect, test } from "bun:test";
import { nextPermutation, permute, permuteUnique } from "./solution";

const norm = (xs: number[][]) => xs.map((p) => p.join(",")).sort();

test("permute", () => {
  expect(norm(permute([1, 2, 3]))).toEqual(norm([[1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1], [3, 1, 2], [3, 2, 1]]));
  expect(permute([1])).toEqual([[1]]);
  expect(permute([1, 2, 3, 4, 5, 6, 7]).length).toBe(5040);
});

test("permuteUnique", () => {
  expect(norm(permuteUnique([1, 1, 2]))).toEqual(norm([[1, 1, 2], [1, 2, 1], [2, 1, 1]]));
  expect(permuteUnique([2, 2, 2, 2]).length).toBe(1);
  expect(permuteUnique([1, 1, 2, 2, 3]).length).toBe(30);
});

test("nextPermutation", () => {
  const cases: [number[], number[]][] = [
    [[1, 2, 3], [1, 3, 2]],
    [[3, 2, 1], [1, 2, 3]],
    [[1, 1, 5], [1, 5, 1]],
    [[1, 3, 2], [2, 1, 3]],
    [[2, 3, 1], [3, 1, 2]],
  ];
  for (const [input, want] of cases) {
    const a = [...input];
    nextPermutation(a);
    expect(a).toEqual(want);
  }
});
