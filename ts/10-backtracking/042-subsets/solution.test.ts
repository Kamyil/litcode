import { expect, test } from "bun:test";
import { subsets, subsetsWithDup } from "./solution";

const norm = (xs: number[][]) => xs.map((s) => [...s].sort((a, b) => a - b).join(",")).sort();

test("subsets", () => {
  expect(norm(subsets([1, 2, 3]))).toEqual(norm([[], [1], [2], [3], [1, 2], [1, 3], [2, 3], [1, 2, 3]]));
  expect(norm(subsets([0]))).toEqual(norm([[], [0]]));
  expect(subsets(Array.from({ length: 12 }, (_, i) => i)).length).toBe(4096);
});

test("subsetsWithDup", () => {
  expect(norm(subsetsWithDup([1, 2, 2]))).toEqual(norm([[], [1], [1, 2], [1, 2, 2], [2], [2, 2]]));
  expect(norm(subsetsWithDup([4, 4, 4, 1, 4]))).toEqual(
    norm([[], [1], [1, 4], [1, 4, 4], [1, 4, 4, 4], [1, 4, 4, 4, 4], [4], [4, 4], [4, 4, 4], [4, 4, 4, 4]]),
  );
});
