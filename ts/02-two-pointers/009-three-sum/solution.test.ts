import { expect, test } from "bun:test";
import { threeSum } from "./solution";

const norm = (xs: number[][]) => xs.map((t) => t.join(",")).sort();

test("examples", () => {
  expect(norm(threeSum([-1, 0, 1, 2, -1, -4]))).toEqual(norm([[-1, -1, 2], [-1, 0, 1]]));
  expect(threeSum([0, 1, 1])).toEqual([]);
  expect(threeSum([0, 0, 0, 0])).toEqual([[0, 0, 0]]);
});

test("many duplicates", () => {
  expect(norm(threeSum([-2, 0, 0, 2, 2, -2, 1, 1, -1, -1]))).toEqual(
    norm([[-2, 0, 2], [-2, 1, 1], [-1, -1, 2], [-1, 0, 1]]),
  );
});

test("does not mutate the result order inside triplets", () => {
  for (const t of threeSum([3, -2, 1, 0, -1, 2, -3])) expect([...t].sort((a, b) => a - b)).toEqual(t);
});
