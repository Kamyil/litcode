import { expect, test } from "bun:test";
import { listFrom, listToArray } from "../../lib/list";
import { reverseBetween, reverseList, reverseListRecursive } from "./solution";

test("iterative", () => {
  expect(listToArray(reverseList(listFrom([1, 2, 3, 4, 5])))).toEqual([5, 4, 3, 2, 1]);
  expect(listToArray(reverseList(listFrom([1])))).toEqual([1]);
  expect(reverseList(null)).toBe(null);
});

test("iterative handles long lists", () => {
  const n = 200_000;
  const out = listToArray(reverseList(listFrom(Array.from({ length: n }, (_, i) => i))), n);
  expect(out[0]).toBe(n - 1);
  expect(out[n - 1]).toBe(0);
});

test("recursive", () => {
  expect(listToArray(reverseListRecursive(listFrom([1, 2, 3])))).toEqual([3, 2, 1]);
  expect(reverseListRecursive(null)).toBe(null);
});

test("reverseBetween", () => {
  expect(listToArray(reverseBetween(listFrom([1, 2, 3, 4, 5]), 2, 4))).toEqual([1, 4, 3, 2, 5]);
  expect(listToArray(reverseBetween(listFrom([5]), 1, 1))).toEqual([5]);
  expect(listToArray(reverseBetween(listFrom([1, 2, 3]), 1, 3))).toEqual([3, 2, 1]);
  expect(listToArray(reverseBetween(listFrom([1, 2, 3]), 1, 2))).toEqual([2, 1, 3]);
});
