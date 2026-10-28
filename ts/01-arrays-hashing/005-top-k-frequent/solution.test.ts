import { expect, test } from "bun:test";
import { topKFrequent } from "./solution";

test("examples", () => {
  expect(topKFrequent([1, 1, 1, 2, 2, 3], 2)).toEqual([1, 2]);
  expect(topKFrequent([1], 1)).toEqual([1]);
});

test("order by frequency", () => {
  expect(topKFrequent([4, 4, 4, 4, 5, 6, 6, 6, 7, 7], 3)).toEqual([4, 6, 7]);
  expect(topKFrequent([-1, -1, 2], 1)).toEqual([-1]);
});
