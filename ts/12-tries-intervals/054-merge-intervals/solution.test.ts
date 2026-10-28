import { expect, test } from "bun:test";
import { insert, merge } from "./solution";

test("merge", () => {
  expect(merge([[1, 3], [2, 6], [8, 10], [15, 18]])).toEqual([[1, 6], [8, 10], [15, 18]]);
  expect(merge([[1, 4], [4, 5]])).toEqual([[1, 5]]);
  expect(merge([[1, 4], [0, 4]])).toEqual([[0, 4]]);
  expect(merge([[1, 4], [2, 3]])).toEqual([[1, 4]]);
  expect(merge([])).toEqual([]);
});

test("merge does not mutate the input", () => {
  const input: [number, number][] = [[5, 6], [1, 2]];
  merge(input);
  expect(input).toEqual([[5, 6], [1, 2]]);
});

test("insert", () => {
  expect(insert([[1, 3], [6, 9]], [2, 5])).toEqual([[1, 5], [6, 9]]);
  expect(insert([[1, 2], [3, 5], [6, 7], [8, 10], [12, 16]], [4, 8])).toEqual([[1, 2], [3, 10], [12, 16]]);
  expect(insert([], [5, 7])).toEqual([[5, 7]]);
  expect(insert([[1, 5]], [6, 8])).toEqual([[1, 5], [6, 8]]);
  expect(insert([[3, 5]], [1, 2])).toEqual([[1, 2], [3, 5]]);
});
