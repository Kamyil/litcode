import { expect, test } from "bun:test";
import { listFrom, listToArray } from "../../lib/list";
import { mergeKLists } from "./solution";

test("examples", () => {
  expect(listToArray(mergeKLists([listFrom([1, 4, 5]), listFrom([1, 3, 4]), listFrom([2, 6])]))).toEqual([
    1, 1, 2, 3, 4, 4, 5, 6,
  ]);
  expect(mergeKLists([])).toBe(null);
  expect(mergeKLists([null, null])).toBe(null);
});

test("many lists", () => {
  const lists = Array.from({ length: 500 }, (_, i) => listFrom(Array.from({ length: 40 }, (_, j) => j * 500 + i)));
  const out = listToArray(mergeKLists(lists), 30_000);
  expect(out.length).toBe(20_000);
  expect(out).toEqual(Array.from({ length: 20_000 }, (_, i) => i));
});
